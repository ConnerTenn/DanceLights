
class InputController;

#ifndef _INPUT_CONTROLLER_H_
#define _INPUT_CONTROLLER_H_

#include "mad.h"
#include <fstream>
#include "Globals.h"
#include "DanceTypes.h"
#include "Spectrum.h"
#include "DanceController.h"

class InputController
{
public:
	static bool Run;
	const static int CallbackLen = 128;
	const static int BufferLen = 1024;
	const static int HistLen = 512;
	//const static int SampleRate = 44100;
	//const static double BufferDuration;
	
	static RoundBuffer<double> SampleBuffer;
	static RoundBuffer<Array<Complex, BufferLen/2>> SpectrumHist;
	
	static void Callback(double samples[CallbackLen]);
	static void StageSamples(double *samples, int length);

	static void InputFromFile(std::string filename);
};

#endif
