
#include "DanceLight.h"
#include "Spectrum.h"
#include <portaudio.h>
#include <thread>
#include <atomic>


double Adj(double val)
{
	return (val>=0?1:-1) * pow(abs(val),0.6);
}

PaError PrintPaError(PaError error)
{
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", error );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( error ) );
	return error;
}


#define SAMPLE_RATE (44100/10)//11)
#define SAMPLE_LEN 64
#define BUFFER_LEN 512
#define SPECTRUM_HIST_LEN 64
#define REDUCED_BANDS 16
#define FREQ(f,t) 2*PI*((double)(t)*(f)/BUFFER_LEN)
#define GRAPH_HEIGHT 8


void DrawGraph(Screen &screen, int x, int y, RoundBuffer<Complex> &data, int len, RGB colour, double scale)
{
	for (int i = 0; i <= GRAPH_HEIGHT; i++) 
	{
		int h = abs(i-GRAPH_HEIGHT/2);
		screen.DrawToBuffer(x, i+y, colour, {0,0,0}, (h<10?h+'0':'-'));
	}
	for (int i = 0; i < len; ++i)
	{
		screen.DrawToBuffer(i+1+x, GRAPH_HEIGHT/2+y, colour, {0,0,0}, i+'0');
		
		//scale = pow(abs(Mag(data[i])), 0.5)*5;
		float dat = scale*Adj(data[i].real());
		for (int j = MAX(-GRAPH_HEIGHT/2,MIN((int)dat, 0)); j <= MIN(GRAPH_HEIGHT/2,MAX((int)dat, 0)); j++)
		{
			if (j == 0) 
			{ 
				if (abs(dat) < 0.5) { screen.DrawToBuffer(i+1+x, GRAPH_HEIGHT/2-j+y, colour, {0,0,0}, (i<10?i+'0':'-')); }
				else { screen.DrawToBuffer(i+1+x, GRAPH_HEIGHT/2-j+y, {0,0,0}, colour, (i<10?i+'0':'-')); }
			}
			else { screen.DrawToBuffer(i+1+x, GRAPH_HEIGHT/2-j+y, colour, colour, ' '); }
		}
	}
}

void DrawDBGraph(Screen &screen, int x, int y, RoundBuffer<Complex> &data, int len, RGB colour, double scale)
{
	for (int i = 0; i <= GRAPH_HEIGHT; i++) 
	{
		screen.DrawToBuffer(x, GRAPH_HEIGHT-i+y, colour, {0,0,0}, (i<10?i+'0':'-'));
	}
	for (int i = 0; i < len; ++i)
	{
		screen.DrawToBuffer(i+1+x, GRAPH_HEIGHT+y, colour, {0,0,0}, (i<10?i+'0':'-'));
		
		//int real = data[i].real(), imag = data[i].imag();
		//screen.Draw(i+1+x, 9-real+y, colour, {0,0,0}, 'R');
		//screen.Draw(i+1+x, 9-imag+y, colour, {0,0,0}, 'I');
		
		//scale = pow(abs(Mag(data[i])), 0.5)*5;
		int dat = (int)(scale*2.0*Adj(Mag(data[i])));
		for (int j = 1; j <= MIN(GRAPH_HEIGHT,dat); j++)
		{
			//if (j == 0) { screen.Draw(i+1+x, 18-j+y, {0,0,0}, colour, (i<10?i+'0':'-')); }
			//else if (j==real) { screen.Draw(i+1+x, 9-j+y, {0,0,0}, colour, 'R'); }
			//else if (j==imag) { screen.Draw(i+1+x, 9-j+y, {0,0,0}, colour, 'I'); }
			//else { screen.Draw(i+1+x, 18-j+y, colour, colour, ' '); }
			screen.DrawToBuffer(i+1+x, GRAPH_HEIGHT-j+y, colour, colour, ' ');
		}
		
	}
}

void DrawSpectrumGraph(Screen &screen, int x, int y, RoundBuffer<Array<Complex, BUFFER_LEN/2>> &data, int len, RGB colour, double scale)
{
	for (int i = 0; i < len; i++)
	{
		for (int j = 0; j < data[i].Size(); j++)
		{
			double dat = scale*Adj(Mag(data[i][j]));
			if (dat > 1.0) { dat = 1.0; }
			colour = RGBFade(dat);
			screen.DrawToBuffer(j+x, i/2+y, {0,0,0}, colour, ' ');
		}
	}
}

void DrawBeatGraph(Screen &screen, int x, int y, Array<Array<Complex, SPECTRUM_HIST_LEN>, BUFFER_LEN/2> &data, int len, RGB colour, double scale)
{
	for (int i = 0; i < BUFFER_LEN/2; i++)
	{
		for (int j = 0; j < len; j++)
		{
			double dat = scale*Adj(Mag(data[i][j]));
			if (dat > 1.0) { dat = 1.0; }
			colour = RGBFade(dat);
			
			screen.DrawToBuffer(i+x, j+y, {255,255,255}, colour, '.');
		}
	}
}

void DrawSpectrumReduced(Screen &screen, int x, int y, RoundBuffer<Array<Complex, REDUCED_BANDS>> &data, int len, RGB colour, double scale)
{
	for (int i = 0; i < len; i++)
	{
		for (int j = 0; j < data[i].Size(); j++)
		{
			double dat = scale*Adj(Mag(data[i][j]));
			if (dat > 1.0) { dat = 1.0; }
			colour = RGBFade(dat);
			screen.DrawToBuffer(x+i, y+j, {0,0,0}, colour, ' ');
		}
	}
}

void DrawBeatGraph2(Screen &screen, int x, int y, Array<Array<Complex, SPECTRUM_HIST_LEN>, REDUCED_BANDS> &data, int len, RGB colour, double scale)
{	
	for (int i = 0; i < REDUCED_BANDS; i++)
	{
		for (int j = 0; j < len; j++)
		{
			double dat = scale*Adj(Mag(data[i][j]));
			if (dat > 1.0) { dat = 1.0; }
			colour = RGBFade(dat);
			
			screen.DrawToBuffer(j+x, i+y, {255,255,255}, colour, '.');
		}
	}
}


RoundBuffer<Complex> InputBuffer(BUFFER_LEN);
RoundBuffer<Complex> FftBuffer(BUFFER_LEN);
RoundBuffer<Array<Complex, BUFFER_LEN/2>> SpectrumBuff(SPECTRUM_HIST_LEN);
Array<Array<Complex, SPECTRUM_HIST_LEN>, BUFFER_LEN/2> BeatBuffer;
RoundBuffer<Array<Complex, REDUCED_BANDS>> ReducedSpectrum(SPECTRUM_HIST_LEN);
Array<Array<Complex, SPECTRUM_HIST_LEN>, REDUCED_BANDS> ReducedBeat;

Screen TextScreen(BUFFER_LEN+1, 2*(GRAPH_HEIGHT+2)+SPECTRUM_HIST_LEN+1+SPECTRUM_HIST_LEN/2+1);
std::thread Thread;
std::atomic<bool> Done = true;


void DoDraw(RoundBuffer<Complex> inputBuffer, RoundBuffer<Complex> fftBuffer, RoundBuffer<Array<Complex, BUFFER_LEN/2>> spectrumBuff, Array<Array<Complex, SPECTRUM_HIST_LEN>, BUFFER_LEN/2> beatBuffer)
{
	TextScreen.Clear();
	TextScreen.ClearBuffer();
	DrawGraph(TextScreen, 0,0, inputBuffer, MIN(inputBuffer.Size(), TextScreen.GetWidth()), {255,0,0}, 2.0);
	
	DrawDBGraph(TextScreen, 0, GRAPH_HEIGHT+1, fftBuffer, fftBuffer.Size()/2, {0,255,0}, 10.0);
	
	DrawSpectrumGraph(TextScreen, 0, 2*(GRAPH_HEIGHT+1), spectrumBuff, spectrumBuff.Size(), {0,0,255}, 1.0);
	
	//DrawBeatGraph(TextScreen, 0, 2*(GRAPH_HEIGHT+1)+SPECTRUM_HIST_LEN/2+1, beatBuffer, SPECTRUM_HIST_LEN/2, {255,255,0}, 3.0);
	
	DrawSpectrumReduced(TextScreen, 0, 2*(GRAPH_HEIGHT+1)+SPECTRUM_HIST_LEN/2+1, ReducedSpectrum, ReducedSpectrum.Size(), {0,255,255}, 2.0);
	
	DrawBeatGraph2(TextScreen, SPECTRUM_HIST_LEN+1, 2*(GRAPH_HEIGHT+1)+SPECTRUM_HIST_LEN/2+1, ReducedBeat, SPECTRUM_HIST_LEN/2, {255,255,0}, 3.0);
	
	
	TextScreen.DisplayBuffer();
	Done = true;
}

void ReduceSpectrumBuffer(RoundBuffer<Complex> &buffer)
{
	//RoundBuffer<Array<Complex, REDUCED_BANDS>> reduced;
	ReducedSpectrum.Append(Array<Complex, REDUCED_BANDS>());
	for (int i=0; i<REDUCED_BANDS; i++)
	{
		ReducedSpectrum[0][i] = 0;
	}
	for (int i=0; i<BUFFER_LEN/2; i++)
	{
		ReducedSpectrum[0][REDUCED_BANDS*i/(BUFFER_LEN/2)] += buffer[i]/(double)REDUCED_BANDS;
	}
}

static int Callback( const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData )
{
	float *in = (float*)inputBuffer;
	(void) outputBuffer; /* Prevent unused variable warning. */
	
	for(unsigned int i=0; i<framesPerBuffer; i++ )
	{
		InputBuffer.Append(in[i]*2.0);
	}
	
	FftBuffer = InputBuffer;
	fft2(FftBuffer);
	SpectrumBuff.Append(Array<Complex, BUFFER_LEN/2>());
	for (int i = 0; i < BUFFER_LEN/2; i++) { SpectrumBuff[0][i] = FftBuffer[i]; }
	
	for (int i = 0; i < BUFFER_LEN/2; i++)
	{
		for (int j = 0; j < SPECTRUM_HIST_LEN; j++)
		{
			BeatBuffer[i][j] = Mag(SpectrumBuff[j][i]);
		}
		
		fft3(BeatBuffer[i]);
	}
	
	ReduceSpectrumBuffer(FftBuffer);
	
	for (int i = 0; i < REDUCED_BANDS; i++)
	{
		for (int j = 0; j < SPECTRUM_HIST_LEN; j++)
		{
			ReducedBeat[i][j] = Mag(ReducedSpectrum[j][i]);
		}
		
		fft3(ReducedBeat[i]);
	}
	
	if (Done == true) 
	{ 
		Done = false; 
		if (Thread.joinable()) { Thread.join(); }
		
		//Thread = std::thread(DoDraw, AudioBuff);
		Thread = std::thread(DoDraw, InputBuffer, FftBuffer, SpectrumBuff, BeatBuffer);
	}
	
	return 0;
}


int main()
{
	/*for (int i = 0; i < LEN/2; i++)
	{
		BeatBuffer[i]=RoundBuffer<Complex>(16);
	}*/
	
	set_conio_terminal_mode();
	
	PaStream *stream;
	PaError error;

	error = Pa_Initialize();
	
	PaStreamParameters inputParameters;
	inputParameters.device = Pa_GetDefaultInputDevice();
	inputParameters.channelCount = 1;
	inputParameters.sampleFormat = paFloat32;// | paNonInterleaved;
	inputParameters.suggestedLatency = 1;
	inputParameters.hostApiSpecificStreamInfo = 0;

	error = Pa_OpenStream(&stream, &inputParameters, 0, SAMPLE_RATE, SAMPLE_LEN, paNoFlag, &Callback, 0);
	
	//error = Pa_OpenStream() &stream, 1, 0, paFloat32, SAMPLE_RATE, LEN, Callback, 0);
	if( error != paNoError ) { return PrintPaError(error); }
	error = Pa_StartStream( stream );
	if( error != paNoError ) { return PrintPaError(error); }
	
	while(!kbhit()) {}
	getch();
	
	error = Pa_StopStream( stream );
	if( error != paNoError ) { return PrintPaError(error); }
	error = Pa_CloseStream( stream );
	if( error != paNoError ) { return PrintPaError(error); }
	Pa_Terminate();
	
	if (Thread.joinable()) { Thread.join(); }
	
	printf("\nTest finished.\n");
	
	reset_terminal_mode();
	return error;
}