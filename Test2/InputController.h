
class InputController;

#ifndef _INPUT_CONTROLLER_H_
#define _INPUT_CONTROLLER_H_

#include "mad.h"
#include <portaudio.h>
#include <fstream>
#include "Globals.h"
#include "DanceTypes.h"
#include "Spectrum.h"
#include "DanceController.h"
#include "TextScreen.h"

class InputController
{
public:
	static bool Run;
	const static int CallbackLen = 128;
	const static int BufferLen = 512;
	const static int HistLen = 64;//512;
	//const static int SampleRate = 44100;
	//const static double BufferDuration;
	
	//samples in range [-1, +1]
	
	static RoundBuffer<double> SampleBuffer;
	static RoundBuffer<Array<double, BufferLen/2>> SpectrumRawHist;
	static RoundBuffer<Array<double, BufferLen/2>> SpectrumHist;
	
	static void Callback(double samples[CallbackLen]);
	template<class T>
	static void StageSamples(T *samples, int length, int compress=1);

	static void InputFromFile(std::string filename);
	static void InputFromMic();
	static void InputFromMath();
};

#endif
