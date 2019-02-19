
#include "Globals.h"
#include "WindowController.h"

struct Style
{
	//Fade, hold, streak, 
	int Speed;
};

struct Cycle
{
	u64 Frequency = 0;
	u64 Offset = 0;
	
	bool SymmetricError = true;
	bool OncePerCycle = false;
	
	u64 OnTime = 0, LastPulse = 0;
	bool PulseState = false;
	void PulseOn(u64 time);
	void PulseOff(u64 time);
	void Pulse(u64 time);
	
	bool Triggered = false;
	bool operator()(u64 time, u64 error = 2);
};

class DanceController
{
public:
	u8 StateIn[4];
	u8 BeatIn;
	
	int UpdateFreq = 50;
	u64 Start, Now, Last, Delta;
	
	Cycle UpdateCycle, Beat;
	
	RoundBuffer<Array<u8,5>> StateHist;
	
	DanceController();
	
	void Update();
	void Draw(int xOff, int yOff);
	
};

