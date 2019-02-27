
struct Cycle;
class DanceController;

#ifndef _DANCE_CONTROLLER_H_
#define _DANCE_CONTROLLER_H_

#include "Globals.h"
#include "WindowController.h"
#include "LightStrip.h"

double Bistable(double x);
double Bistable(double x, double a);
double ASD(double x, double a, double s, double d);
double ASDR(double x, double a, double s, double d, double r, double f);
double RoundMean(double a, double b, double m, double w = 0.5);
i8 RoundDirection(double a, double b, double m);

struct Cycle
{
	u64 Period = 0;
	u64 Align = 0;
	
	bool ActOnPulseOn = false;
	
	Cycle(int len=1);
	
	u64 OnTime = 0, LastAccept = 0;
	RoundBuffer<u64> PulseHist;
	bool PulseState = false;
	void PulseOn(u64 time);
	void PulseOff(u64 time);
	void Pulse(u64 time);
	
	//bool Triggered = false;
	bool operator()(u64 time, u64 error = 2, bool symmetricError = false, bool *latch = 0);
};

struct ColourTimestamp
{
	u64 Time;
	u64 Attack;
	ColourVal Colour;
};

class DanceController
{
public:
	u8 StateIn[4];
	u8 BeatIn;
	
	double UpdateFreq = 50;
	u64 Start, Now;//, Last, Delta;
	
	Cycle UpdateCycle, Beat, MulBeat;
	bool HalfTime = false, DoubleTime = false;
	
	RoundBuffer<Array<u8,5>> StateHist;
	
	std::vector<LightStrip> LightStripList;
	
	std::vector<ColourTimestamp> ColourHist;
	
	DanceController();
	
	void Update();
	void Draw(int xOff, int yOff);
	
	RGB GetColour(u64 now);
	
};

#endif
