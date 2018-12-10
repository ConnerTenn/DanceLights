
#include <stdio.h>
#include <math.h>
#include <portaudio.h>
#define NUM_SECONDS   (10)
#define SAMPLE_RATE   (44100)
#define MIN(a,b) ((a)<=(b)?(a):(b))
#define MAX(a,b) ((a)>=(b)?(a):(b))

PaError PrintPaError(PaError error)
{
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", error );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( error ) );
	return error;
}

void PrintAmp(float val)
{
	printf("\r[");
	int v = val*20;
	for (int i =0; i <= 20; i++)
	{
		if (v>=i) { printf("#"); }
		else { printf(" "); }
	}
	printf("]");
}

/* This routine will be called by the PortAudio engine when audio is needed.
** It may called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
static int patestCallback( const void *inputBuffer, void *outputBuffer,
						   unsigned long framesPerBuffer,
						   const PaStreamCallbackTimeInfo* timeInfo,
						   PaStreamCallbackFlags statusFlags,
						   void *userData )
{
	float *in = (float*)inputBuffer;
	(void) outputBuffer; /* Prevent unused variable warning. */
	
	float max = 0;
	for(unsigned int i=0; i<framesPerBuffer; i++ )
	{
		max = (abs(in[i]) > max ? abs(in[i]) : max);
	}
	PrintAmp(pow(max, 0.5));
	return 0;
}

/*******************************************************************/
int main(void)
{
	PaStream *stream;
	PaError error;

	printf("PortAudio Test: output sawtooth wave.\n");
	/* Initialize our data for use by callback. */
	/* Initialize library before making any other calls. */
	error = Pa_Initialize();


	/* Open an audio I/O stream. */
	error = Pa_OpenDefaultStream( &stream,
								1,          /* no input channels */
								0,          /* stereo output */
								paFloat32,  /* 32 bit floating point output */
								SAMPLE_RATE,
								32,        /* frames per buffer */
								patestCallback,
								0);
	if( error != paNoError ) { return PrintPaError(error); }

	error = Pa_StartStream( stream );
	if( error != paNoError ) { return PrintPaError(error); }

	/* Sleep for several seconds. */
	Pa_Sleep(NUM_SECONDS*1000);

	error = Pa_StopStream( stream );
	if( error != paNoError ) { return PrintPaError(error); }
	error = Pa_CloseStream( stream );
	if( error != paNoError ) { return PrintPaError(error); }
	Pa_Terminate();

	printf("\nTest finished.\n");
	return error;
}
