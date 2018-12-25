
#include "InputController.h"


bool InputController::Run = true;
//const double InputController::BufferDuration = (double)InputController::BufferLen * 1.0/(double)InputController::SampleRate;

RoundBuffer<double> InputController::SampleBuffer(InputController::BufferLen);
RoundBuffer<Array<Complex, InputController::BufferLen/2>> InputController::SpectrumHist;
	
//Screen TextScreen(200, 21);

void InputController::Callback(double samples[CallbackLen])
{
	std::cout << "Callback\n";
	
	for (int i = 0; i < CallbackLen; i++)
	{
		SampleBuffer.InsertBegin(samples[i]);
	}
	
	Complex spectrum[BufferLen];
	for (int i = 0; i < BufferLen; i++) { spectrum[i] = SampleBuffer[i]; }
	FftInplace(spectrum, BufferLen);
	Array<Complex, BufferLen/2> spectrum2;
	for (int i = 0; i < BufferLen/2; i++) { spectrum2[i] = spectrum[i]; }
	SpectrumHist.InsertBegin(spectrum2);
	
}

double StagingBuffer[InputController::BufferLen];
u32 StagingFill = 0;

void InputController::StageSamples(double *samples, int length)
{
	std::cout << "StageSamples\n";
	for (int i = 0; i < length; i++)
	{
		StagingBuffer[StagingFill] = samples[i];
		StagingFill++;
		
		if (StagingFill == BufferLen)
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

static mad_flow input(void *data, mad_stream *stream);
static mad_flow output(void *data, mad_header const *header, mad_pcm *pcm);
static mad_flow error(void *data, mad_stream *stream, mad_frame *frame);

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
	mad_decoder_init(&decoder, &buffer, input, 0 /* header */, 0 /* filter */, output, error, 0 /* message */);
	
	//std::cout << "MAD Decode\n";
	mad_decoder_run(&decoder, MAD_DECODER_MODE_SYNC);
	
	//std::cout << "MAD Close\n";
	mad_decoder_finish(&decoder);
	
	delete[] buffer.start;
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


static mad_flow input(void *data, mad_stream *stream)
{
	Buffer *buff = (Buffer *)data;
	//std::cout << "Read Data\n";
	
	if (!buff->length) { return MAD_FLOW_STOP; }
	
	mad_stream_buffer(stream, (const u8 *)buff->start, buff->length);
	
	buff->length = 0;
	
	return MAD_FLOW_CONTINUE;
}

static mad_flow output(void *data, mad_header const *header, mad_pcm *pcm)
{
	unsigned int nchannels, nsamples;

	/* pcm->samplerate contains the sampling frequency */
	nchannels = pcm->channels;
	nsamples = pcm->length;
	
	double samples[1152];
	
	for (u32 i = 0; i < nsamples; i++)
	{
		samples[i] = (double)pcm->samples[0][i]/MAD_F_ONE;
		if (nchannels==2) { samples[i] += (double)pcm->samples[1][i]/MAD_F_ONE; samples[i] = samples[i]/2.0; }
		//samples[i] = scale(samples[i]);
	}
	
	if (InputController::Run)
	{
		InputController::StageSamples(samples,nsamples);
	}	
	else { return MAD_FLOW_STOP; }
	
	usleep(500*1000);
	
	return MAD_FLOW_CONTINUE;
}
static mad_flow error(void *data, mad_stream *stream, mad_frame *frame)
{
	std::cerr << "ERROR: decoding error 0x"<<stream->error<<" ("<<mad_stream_errorstr(stream)<<") at byte offset "<<(stream->this_frame - (u8 *)((Buffer *)data)->start)<<"\n";
	
	return MAD_FLOW_CONTINUE;
}