
struct Cycle;
struct Streak;
struct Fade;
class DanceController;

#ifndef _DANCE_CONTROLLER_H_
#define _DANCE_CONTROLLER_H_

#include "Globals.h"
#include "WindowController.h"
#include "LightStrip.h"

double Bistable(double x);
double ASD(double x, double a, double s, double d);
double ASDR(double x, double a, double s, double d, double r, double f);
double RoundMean(double a, double b, double m, double w = 0.5);
i8 RoundDirection(double a, double b, double m);
RGB ColourMix(RGB a, RGB b, double w);

struct Style
{
	//slide poly mash flip;    hold pulse, Fade, streak
	bool Hold;
	u8 Pulse; //Pulse can be controlled by streak
	u8 Streak;
	u8 Fade;
	
	u64 Speed; //used for streak
	u64 Period;
	u64 Offset;
	u64 Align;
	double Amplitude;
	
	double Colour;
};

/*struct TargetTracker
{
	int Current;
	int Target;
	int Speed;
	bool Linear = true;
	void Update();
	void operator()();
};*/

struct Cycle
{
	u64 Period = 0;
	u64 Align = 0;
	
	//bool SymmetricError = true;
	//bool OncePerCycle = false;
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

struct Streak
{
	double Attack;
	double Decay;
	double Sustain;
	u64 Align;
	double Speed;
	double Colour;
};

struct Fade
{
	double Target;
	double Colour;
	double Speed;
	void Update(u64 now);
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
	
	std::vector<Streak> StreakList;
	Fade ColourFade;
	
	std::vector<LightStrip> LightStripList;
	
	double PrimaryColour = RED;
	
	DanceController();
	
	void Update();
	void Draw(int xOff, int yOff);
	
};

#endif
