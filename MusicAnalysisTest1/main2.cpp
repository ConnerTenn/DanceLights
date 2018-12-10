
#include "DanceLight.h"
#include "Spectrum.h"
#include <portaudio.h>



double Adj(double val)
{
	return (val>=0?1:-1) * pow(abs(val),0.8);
}

PaError PrintPaError(PaError error)
{
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", error );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( error ) );
	return error;
}


#define SAMPLE_RATE (44100/5)//11)
#define SAMPLE_DEPTH 64
#define LEN 512
#define FREQ(f,t) 2*PI*((double)(t)*(f)/LEN)
#define GRAPH_HEIGHT 10
#define SPECTRUM_HIST 64

void DrawGraph2(Screen &screen, int x, int y, RoundBuffer<Complex> &data, int len, RGB colour, double scale)
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

void DrawDBGraph2(Screen &screen, int x, int y, RoundBuffer<Complex> &data, int len, RGB colour, double scale)
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

void DrawSpectrumGraph(Screen &screen, int x, int y, RoundBuffer<Array<Complex, LEN/2>> &data, int len, RGB colour, double scale)
{
	for (int i = 0; i < len; i++)
	{
		for (int j = 0; j < data[i].Size(); j++)
		{
			double dat = scale*Adj(Mag(data[i][j]));
			if (dat > 1.0) { dat = 1.0; }
			colour = RGBFade(dat);
			screen.DrawToBuffer(j+x, i+y, {0,0,0}, colour, ' ');
		}
		
	}
}
/*
void DrawBeatGraph2(Screen &screen, int x, int y, RoundBuffer<RoundBuffer<Complex>> &data, RGB colour, double scale)
{
	for (int i = 0; i <= GRAPH_HEIGHT; i++) 
	{
		screen.DrawToBuffer(x, GRAPH_HEIGHT-i+y, colour, {0,0,0}, (i<10?i+'0':'-'));
	}
	for (int i = 0; i < (int)data.Size()/2; ++i)
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
}*/

#include <thread>
#include <atomic>

RoundBuffer<Complex> AudioBuff2(LEN);
//RoundBuffer<RoundBuffer<Complex>> BeatBuffer(LEN/2);
RoundBuffer<Array<Complex, LEN/2>> SpectrumBuff(SPECTRUM_HIST);
Screen TextScreen(LEN+1, 2*(GRAPH_HEIGHT+2)+SPECTRUM_HIST+1);
std::thread Thread;
std::atomic<bool> Done = true;

void DoDraw2(RoundBuffer<Complex> buff)
{
	TextScreen.Clear();
	TextScreen.ClearBuffer();
	DrawGraph2(TextScreen, 0,0, buff, MIN(buff.Size(), TextScreen.GetWidth()), {255,0,0}, 30.0);
	fft2(buff);
	DrawDBGraph2(TextScreen, 0, GRAPH_HEIGHT+1, buff, buff.Size()/2, {0,255,0}, 50.0);
	
	//ifft2(buff);
	//DrawGraph2(TextScreen, 0,40, buff, {0,100,255}, 15.0);
	//DrawBeatGraph2(TextScreen, 0,2*(GRAPH_HEIGHT+2), BeatBuffer, {0,0,255}, 100.0);
	//for (int i = 0; i < LEN/2; i++) { BeatBuffer[i].Append(buff[i]); }
	//RoundBuffer<Complex> buff2 = beat(BeatBuffer);
	//DrawDBGraph2(TextScreen, 0,2*(GRAPH_HEIGHT+2), buff2, buff2.Size(), {0,0,255}, 100.0);
	
	
	//SpectrumBuff.Append(Array<Complex, LEN/2>());
	//for (int i = 0; i < LEN/2; i++) { SpectrumBuff[0][i] = buff[i]; }
	DrawSpectrumGraph(TextScreen, 0, 2*(GRAPH_HEIGHT+1), SpectrumBuff, SpectrumBuff.Size(), {0,0,255}, 5.0);
	
	//for (int i = 0; i < 256; i++) { TextScreen.DrawToBuffer(i, 2*(GRAPH_HEIGHT+2)-1, {0,0,0}, RGBFade(i/255.0), ' '); }
	
	TextScreen.DisplayBuffer();
	Done = true;
}

static int Callback( const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData )
{
	float *in = (float*)inputBuffer;
	(void) outputBuffer; /* Prevent unused variable warning. */
	
	for(unsigned int i=0; i<framesPerBuffer; i++ )
	{
		AudioBuff2.Append(in[i]*4.0);
	}
	
	RoundBuffer<Complex> buff(AudioBuff2);
	fft2(buff);
	SpectrumBuff.Append(Array<Complex, LEN/2>());
	for (int i = 0; i < LEN/2; i++) { SpectrumBuff[0][i] = buff[i]; }
	
	if (Done == true) 
	{ 
		Done = false; 
		if (Thread.joinable()) { Thread.join(); }
		
		//Thread = std::thread(DoDraw, AudioBuff);
		Thread = std::thread(DoDraw2, AudioBuff2);
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

	error = Pa_OpenStream(&stream, &inputParameters, 0, SAMPLE_RATE, SAMPLE_DEPTH, paNoFlag, &Callback, 0);
	
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

int main2()
{
	RoundBuffer<int> buff(5);
	
	PRINT((-4%3)<<"\n");
	buff.Append(1);
	PRINT(buff[0] << "\n\n");
	for (int i = 2; i <= 7; i++)
	{
		buff.Append(i);
		
		for (int i = 0; i < 5; i++)
		{
			PRINT(i << ":" << buff[i]<<"\n");
		}
		PRINT("\n");
	}
	return 0;
}
