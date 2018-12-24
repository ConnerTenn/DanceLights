
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
	const static int BufferLen = 512;
	const static int SampleRate = 44100;
	const static double BufferDuration;
	
	static RoundBuffer<int> SampleBuffer;
	
	static void Callback(int samples[BufferLen]);
	static void StageSamples(int *samples, u32 length);

	static void InputFromFile(std::string filename);
};

#endif
