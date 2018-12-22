
#include "Globals.h"
#include <fstream>
#include "mad.h"

struct Buffer 
{
	char *start;
	unsigned long length;
};

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


static enum mad_flow input(void *data, struct mad_stream *stream)
{
	Buffer *buff = (Buffer *)data;
	//std::cout << "Read Data\n";
	
	if (!buff->length) { return MAD_FLOW_STOP; }
	
	mad_stream_buffer(stream, (const u8 *)buff->start, buff->length);
	
	buff->length = 0;
	
	return MAD_FLOW_CONTINUE;
}

static enum mad_flow output(void *data, struct mad_header const *header, struct mad_pcm *pcm)
{	
	//std::cout << "Output Data\n";
	unsigned int nchannels, nsamples;
	mad_fixed_t *channels[2];

	/* pcm->samplerate contains the sampling frequency */

	nchannels = pcm->channels;
	nsamples = pcm->length;
	channels[0] = pcm->samples[0];
	channels[1] = pcm->samples[1];

	while (nsamples--) 
	{
		int sample;

		/* output sample(s) in 16-bit signed little-endian PCM */
		for (unsigned int c = 0; c < nchannels; c++)
		{
			sample = scale(*channels[c]);
			for (int i = 0; i < BITS/8; i++)
			{
				putchar((sample >> i*8) & 0xff);
			}
			
			channels[c]++;
		}
	}
	usleep(10000);
	return MAD_FLOW_CONTINUE;
}
static enum mad_flow error(void *data, struct mad_stream *stream, struct mad_frame *frame)
{
	std::cerr << "ERROR: decoding error 0x"<<stream->error<<" ("<<mad_stream_errorstr(stream)<<") at byte offset "<<(stream->this_frame - (u8 *)((Buffer *)data)->start)<<"\n";
	
	return MAD_FLOW_CONTINUE;
}

void InputFromFile(std::string filename)
{
	Buffer buffer;
	mad_decoder decoder;
	
	//std::cout << "Opening file "<<filename<<"\n";
	
	std::ifstream file(filename, std::ifstream::binary);
	
	if (!file) { /*std::cout << "ERROR: Failed to open file\n";*/ return; }
	
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

int main(int argc, char **argv)
{
	if (argc != 2) { /*std::cout << "ERROR: Incorrect Arguments\n";*/ return 1; } 
	
	//std::cout << "Starting thread\n";
	std::thread inputThread(InputFromFile, std::string(argv[1]));
	
	inputThread.join();
	//std::cout << "Done\n";
	
	return 0;
}