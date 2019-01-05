
#include "InputController.h"


bool InputController::Run = true;
//const double InputController::BufferDuration = (double)InputController::BufferLen * 1.0/(double)InputController::SampleRate;

RoundBuffer<double> InputController::SampleBuffer(InputController::BufferLen);
RoundBuffer<Array<double, InputController::BufferLen/2>> InputController::SpectrumRawHist(InputController::HistLen);
RoundBuffer<Array<double, InputController::BufferLen/2>> InputController::SpectrumHist(InputController::HistLen);

double SoundCurve(double val)
{
	return pow(val,1.0/1.0)*4;
	//return val;//pow(val,2.0);
}
const int WavHeight = 150;
const int WavScale = 2;
void DrawWav(RoundBuffer<double> *wav, int x, int y)
{
	PixelTerm::OutlineRectangle(x, y, InputController::BufferLen*WavScale+2, WavHeight+2, {0xff,0xff,0xff});
	PixelTerm::DrawLine(x+1,y+WavHeight/2,x+1+InputController::BufferLen*WavScale,y+WavHeight/2, {0x00,0x00,0xff});
	for (int i = 0; i < InputController::BufferLen-1; i++)
	{
		PixelTerm::DrawLine(i*WavScale+x+1, WavHeight/2-(*wav)[i]*WavHeight/2+y+1, i*WavScale+1+x+1, WavHeight/2-(*wav)[i+1]*WavHeight/2+y+1, {0x00,0xff,0xff});
	}
}
const int DotWidth = 5;
const int DotHeight = 5;
void DrawHist(RoundBuffer<Array<double, InputController::BufferLen/2>> *hist, int x, int y)
{
	PixelTerm::OutlineRectangle(x, y, DotWidth*InputController::BufferLen/2+2, DotHeight*InputController::HistLen+2, {0xff,0xff,0xff});
	for (int i = 0; i < InputController::HistLen; i++)
	{
		for (int j = 0; j < InputController::BufferLen/2; j++)
		{
			if (ABS((*hist)[i][j]) > 0.1)
			{
				u8 val = MIN(/*SoundCurve*/(ABS((*hist)[i][j]))*255.0,255);
				//std::cout << (ABS(Mag(SpectrumHist[i][j]))*255.0) << " ";
				PixelTerm::DrawRectangle(j*DotWidth+1+x, i*DotHeight+1+y, DotWidth, DotHeight, {0, val, 0});
			}
		}
	}
}

void InputController::Callback(double samples[CallbackLen])
{
	//std::cout << "Callback\n";
	//TermBuffer.Write("Callback");
	
	for (int i = 0; i < CallbackLen; i++)
	{
		SampleBuffer.InsertBegin(samples[i]);
	}
	
	ComplexD spectrum[BufferLen];
	for (int i = 0; i < BufferLen; i++) { spectrum[i] = SampleBuffer[i]; }
	FftInplace(spectrum, BufferLen);
	
	Array<double, BufferLen/2> spectrum2;
	for (int i = 0; i < BufferLen/2; i++) { spectrum2[i] = std::abs(spectrum[i]); }
	SpectrumRawHist.InsertBegin(spectrum2);
	
	double max = 0;
	double avg = 0;
	/*for (int i = 0; i < HistLen; i++)
	{
		double maxRow = 0;
		for (int j = 0; j < BufferLen/2; j++)
		{
			maxRow = MAX(maxRow, std::abs(SpectrumRawHist[i][j]));
			//maxRow += std::abs(SpectrumRawHist[i][j]);
		}
		max = MAX(max, maxRow);
		avg += maxRow;
	}*/
	for (int i = 0; i < BufferLen; i++)
	{
		max = MAX(max, ABS(SampleBuffer[i]));
		avg += ABS(SampleBuffer[i]);
	}
	avg = avg/(BufferLen);
	avg = MIN(MAX(avg, 0.01), 1.0);
	max = MAX(max, avg);
	
	for (int i = 0; i < BufferLen/2; i++) { spectrum2[i] = spectrum2[i]/max; }
	SpectrumHist.InsertBegin(spectrum2);
	
	PixelTerm::ForceClear();
	
	{
		DrawWav(&SampleBuffer, 0, 20);
		DrawHist(&SpectrumRawHist, 0, WavHeight+20+20);
		DrawHist(&SpectrumHist, 0, HistLen*DotHeight+20+WavHeight+20+20);
		
		//int h = HistLen*DotHeight;
		PixelTerm::OutlineRectangle(BufferLen*WavScale+10, 20, 10, WavHeight, {0xff,0xff,0xff});
		PixelTerm::DrawRectangle(BufferLen*WavScale+10, WavHeight-avg*WavHeight+20, 10, WavHeight-(WavHeight-avg*WavHeight), {0x00,0xff,0x00});
		PixelTerm::DrawLine(BufferLen*WavScale+10, WavHeight-max*WavHeight+20, BufferLen*WavScale+10+10, WavHeight-max*WavHeight+20, {0xff,0xff,0x00});
		/*PixelTerm::OutlineRectangle(0, h*HistLen+20, 100, 100, {0xff,0xff,0xff});
		for (int i = 0; i < 100; i++)
		{
			int val1 = (int)(SoundCurve(i/100.0)*100);
			int val2 = (int)(SoundCurve((i+1)/100.0)*100);
			PixelTerm::DrawLine(i, 100-val1+h*HistLen+20, i+1, 100-val2+h*HistLen+20, {0x00,0xff,0x00});
		}*/
	}
	
	TermBuffer.Display();
	float elapsed = (GetMilliseconds() - StartTime)/1000.00;
	PixelTerm::DrawText(10, 10, std::to_string(elapsed), {0xff,0x4f,0x00});
	
	PixelTerm::Draw();
}

double StagingBuffer[InputController::CallbackLen];
u32 StagingFill = 0;
int CompressionStep = 0;

template<class T>
void InputController::StageSamples(T *samples, int length, int compress)
{
	//std::cout << "StageSamples\n";
	//TermBuffer.Write("StageSamples");
	for (int i = 0; i < length; i++)
	{
		if (CompressionStep == 0) { StagingBuffer[StagingFill] = 0; }
		StagingBuffer[StagingFill] += (double)samples[i];
		CompressionStep = (CompressionStep+1)%compress;
		if (CompressionStep == 0) { StagingBuffer[StagingFill] = StagingBuffer[StagingFill]/compress; StagingFill++; }
		
		if (StagingFill == CallbackLen)
		{
			Callback(StagingBuffer);
			StagingFill = 0;
		}
	}
}





struct Buffer
{
	char *start;
	unsigned long length;
};

static mad_flow MadInput(void *data, mad_stream *stream);
static mad_flow MadOutput(void *data, mad_header const *header, mad_pcm *pcm);
static mad_flow MadError(void *data, mad_stream *stream, mad_frame *frame);
void PAudioError(PaError error);
void SetupOutputSpeaker(int sampleRate);
void CloseOutputSpeaker();
static int PAudioInputCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData);
static int PAudioOutputCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData);

void InputController::InputFromFile(std::string filename)
{
	Buffer buffer;
	mad_decoder decoder;
	
	std::cout << "Opening file "<<filename<<"\n";
	
	std::ifstream file(filename, std::ifstream::binary);
	
	if (!file) { std::cerr << "ERROR: Failed to open file\n"; return; }
	
	file.seekg(0, file.end);
	buffer.length = file.tellg();
	file.seekg(0, file.beg);
	buffer.start = new char[buffer.length];
	file.read(buffer.start, buffer.length);
	//std::cout << "File has " << buffer.length << " bytes\n";
	
	//std::cout << "MAD Init\n";
	mad_decoder_init(&decoder, &buffer, MadInput, 0 /* header */, 0 /* filter */, MadOutput, MadError, 0 /* message */);
	
	SetupOutputSpeaker(44100);
	//std::cout << "MAD Decode\n";
	mad_decoder_run(&decoder, MAD_DECODER_MODE_SYNC);
	
	CloseOutputSpeaker();
	
	//std::cout << "MAD Close\n";
	mad_decoder_finish(&decoder);
	
	delete[] buffer.start;
}

#define MICSAMPLERATE 44100/2

void InputController::InputFromMic()
{
	PaStream *stream;
	PaError error;
	
	std::cout << "Setting up Microphone\n";

	error = Pa_Initialize();
	
	PaStreamParameters inputParameters;
	inputParameters.device = Pa_GetDefaultInputDevice();
	inputParameters.channelCount = 1;
	inputParameters.sampleFormat = paFloat32;// | paNonInterleaved;
	inputParameters.suggestedLatency = 1;
	inputParameters.hostApiSpecificStreamInfo = 0;

	error = Pa_OpenStream(&stream, &inputParameters, 0, (MICSAMPLERATE), InputController::BufferLen, paNoFlag, &PAudioInputCallback, 0);
	
	//error = Pa_OpenStream() &stream, 1, 0, paFloat32, SAMPLE_RATE, LEN, Callback, 0);
	if( error != paNoError ) { return PAudioError(error); }
	error = Pa_StartStream( stream );
	if( error != paNoError ) { return PAudioError(error); }
	
	while (InputController::Run) { usleep(1000); }
	
	std::cout << "Stopping Microphone\n";
	
	error = Pa_StopStream( stream );
	if( error != paNoError ) { return PAudioError(error); }
	error = Pa_CloseStream( stream );
	if( error != paNoError ) { return PAudioError(error); }
	Pa_Terminate();
}

void InputController::InputFromMath()
{
	const int sampleRate = 44100;
	
	double (* equation)(double) = [](double val)->double{
		return sin(TAU*val);
	};
	
	while(InputController::Run)
	{
		TermBuffer.Write("Input From Math");
		u64 time = GetMicroseconds();
		double sample = equation(time/500);
		StageSamples(&sample, 1);
		
		static u64 PreviousTime = GetMicroseconds();
		u64 delta = GetMicroseconds() - PreviousTime;
		usleep((1000*1000)/sampleRate*10-delta);
		PreviousTime=GetMicroseconds();
	}
	//StageSamples()
}









#define BITS 16
static inline
signed int scale(mad_fixed_t sample)
{
	/* round */
	sample += (1L << (MAD_F_FRACBITS - BITS));

	/* clip */
	if (sample >= MAD_F_ONE)
		sample = MAD_F_ONE - 1;
	else if (sample < -MAD_F_ONE)
		sample = -MAD_F_ONE;

	/* quantize */
	return sample >> (MAD_F_FRACBITS + 1 - BITS);
}


static mad_flow MadInput(void *data, mad_stream *stream)
{
	Buffer *buff = (Buffer *)data;
	//std::cout << "Read Data\n";
	
	if (!buff->length) { return MAD_FLOW_STOP; }
	
	mad_stream_buffer(stream, (const u8 *)buff->start, buff->length);
	
	buff->length = 0;
	
	return MAD_FLOW_CONTINUE;
}


float OutputBuffer[1152];
static mad_flow MadOutput(void *data, mad_header const *header, mad_pcm *pcm)
{
	unsigned int nchannels, nsamples, sampleRate;

	/* pcm->samplerate contains the sampling frequency */
	nchannels = pcm->channels;
	nsamples = pcm->length;
	sampleRate = pcm->samplerate;
	
	double samples[1152];
	
	for (u32 i = 0; i < nsamples; i++)
	{
		samples[i] = (double)pcm->samples[0][i]/MAD_F_ONE;
		if (nchannels==2) { samples[i] += (double)pcm->samples[1][i]/MAD_F_ONE; samples[i] = samples[i]/2.0; }
		//samples[i] = scale(samples[i]);
		OutputBuffer[i] = samples[i];
	}
	
	if (InputController::Run)
	{
		InputController::StageSamples(samples,nsamples,4);
	}	
	else { return MAD_FLOW_STOP; }
	
	
	{
		static u64 PreviousTime = GetMicroseconds();
		u64 delta = GetMicroseconds() - PreviousTime;
		usleep((1000*1000)/sampleRate*1152-delta);
		PreviousTime=GetMicroseconds();
	}
	
	return MAD_FLOW_CONTINUE;
}
static mad_flow MadError(void *data, mad_stream *stream, mad_frame *frame)
{
	std::cerr << "ERROR: decoding error 0x"<<stream->error<<" ("<<mad_stream_errorstr(stream)<<") at byte offset "<<(stream->this_frame - (u8 *)((Buffer *)data)->start)<<"\n";
	
	return MAD_FLOW_CONTINUE;
}

void PAudioError(PaError error)
{
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", error );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( error ) );
}

PaStream *OutStream;
void SetupOutputSpeaker(int sampleRate)
{
	PaError error;
	
	std::cout << "Setting up Output Speaker\n";

	error = Pa_Initialize();
	
	PaStreamParameters outputParameters;
	outputParameters.device = Pa_GetDefaultOutputDevice();
	outputParameters.channelCount = 1;
	outputParameters.sampleFormat = paFloat32;// | paNonInterleaved;
	outputParameters.suggestedLatency = 1;
	outputParameters.hostApiSpecificStreamInfo = 0;

	error = Pa_OpenStream(&OutStream, 0, &outputParameters, (sampleRate), 1152, paNoFlag, &PAudioOutputCallback, 0);
	
	//error = Pa_OpenStream() &stream, 1, 0, paFloat32, SAMPLE_RATE, LEN, Callback, 0);
	if( error != paNoError ) { return PAudioError(error); }
	error = Pa_StartStream( OutStream );
	if( error != paNoError ) { return PAudioError(error); }
}

void CloseOutputSpeaker()
{
	std::cout << "Stopping output speaker\n";
	
	PaError error;
	error = Pa_StopStream( OutStream );
	if( error != paNoError ) { return PAudioError(error); }
	error = Pa_CloseStream( OutStream );
	if( error != paNoError ) { return PAudioError(error); }
	Pa_Terminate();
}

static int PAudioInputCallback( const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData )
{
	InputController::StageSamples((float*)inputBuffer, framesPerBuffer,2);
	
	return 0;
}

static int PAudioOutputCallback( const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData )
{
	for (int i = 0; i < 1152; i++)
	{
		((float *)outputBuffer)[i] = OutputBuffer[i];
	}
	
	return 0;
}


