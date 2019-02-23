
#ifndef _DANCE_CONTROLLER_H_
#define _DANCE_CONTROLLER_H_

#include "Globals.h"
#include "WindowController.h"

struct Style
{
	//slide poly mash flip;    hold pulse, Fade, streak
	bool Hold;
	u8 Pulse; //Pulse can be controlled by streak
	u8 Streak;
	u8 Fade;
	
	u64 Speed; //used for streak
	u64 Frequency;
	u64 Offset;
	u64 Align;
	double Amplitude;
	
	double Colour;
};

struct Cycle
{
	u64 Frequency = 0;
	u64 Align = 0;
	
	//bool SymmetricError = true;
	bool OncePerCycle = false;
	bool ActOnPulseOn = false;
	
	Cycle(int len=1);
	
	u64 OnTime = 0, LastAccept = 0;
	RoundBuffer<u64> PulseHist;
	bool PulseState = false;
	void PulseOn(u64 time);
	void PulseOff(u64 time);
	void Pulse(u64 time);
	
	bool Triggered = false;
	bool operator()(u64 time, u64 error = 2, bool symmetricError = false);
};

class DanceController
{
public:
	u8 StateIn[4];
	u8 BeatIn;
	
	int UpdateFreq = 50;
	u64 Start, Now;//, Last, Delta;
	
	Cycle UpdateCycle, Beat;
	
	RoundBuffer<Array<u8,5>> StateHist;
	
	DanceController();
	
	void Update();
	void Draw(int xOff, int yOff);
	
};

#endif
