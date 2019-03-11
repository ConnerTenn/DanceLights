

#ifndef ENUM_STYLE
#define ENUM_STYLE

enum Style
{
	Fade, //Light
	Streak, //Medium
	StreakFade, //Heavy-medium
	Pulse, //Heavy
	FlipFlop,
};

#endif

struct Cycle;
class DanceController;

#ifndef _DANCE_CONTROLLER_H_
#define _DANCE_CONTROLLER_H_

#include "Globals.h"
#include "LightStrip.h"

inline double Bistable(double x, double a)
{
	return x<0 ? 0 : (x<a && a!=0 ? x/a : 1);
}
//double ASD(double x, double a, double s, double d);
//double ASDR(double x, double a, double s, double d, double r, double f);
double RoundMean(double a, double b, double m, double w = 0.5);
i8 RoundDirection(double a, double b, double m);


struct Cycle
{
	double Period = 0;
	i64 Align = 0;
	
	bool ActOnPulseOn = false;
	
	Cycle(int len=1);
	
	i64 OnTime = 0, LastAccept = 0;
	RoundBuffer<i64> PulseHist;
	bool PulseState = false;
	void PulseOn(i64 time);
	void PulseOff(i64 time);
	void Pulse(i64 time);
	
	//bool Triggered = false;
	bool operator()(i64 time, double error = 1, bool symmetricError = false, bool *latch = 0);
};

struct ColourTimestamp
{
	i64 Time;
	i64 Attack;
	ColourVal Colour;
};

class DanceController
{
public:
	u8 StateIn[4];
	u8 BeatIn;
	
	i64 Start, Now;//, Last, Delta;
	
	Cycle UpdateCycle, Beat, MulBeat;
	int Speed = 0;
	
	RoundBuffer<Array<u8,5>> StateHist;
	
	std::vector<LightStrip> LightStripList;
	
	std::vector<ColourTimestamp> ColourHist;
	
	int MajorWeight = 0;
	//double MinorWeight = 0;
	Style CurrStyle = Style::Fade, NextStyle = Style::Fade;
	
	bool Hold = false; 
	i64 HoldAlign = 0, GlobalDelay = 0;
	bool Manual = false, ForceUpdate = false;
	
	bool FlipFlop = false;
	//i64 Oldest = 0;
	
	DanceController();
	
	void Update();
	
	inline RGB GetColour(i64 now)
	{
		RGB colour = {0,0,0};
		//if (now < Oldest) { return colour; }
		
		int max=ColourHist.size();
		int i = max-1;
		for (; i>=0 && ColourHist[i].Time > now; i--) { }
		if (max)
		{
			double mix = Bistable(now-ColourHist[i].Time, ColourHist[i].Attack);
			colour = ColourMix(i == 0 ? colour : RGBVal(ColourHist[i-1].Colour), RGBVal(ColourHist[i].Colour), mix);
		}
		
		return colour;
	}
	
	ColourVal ColourPicker();
};

#endif
