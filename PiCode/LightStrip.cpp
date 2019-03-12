
#include "LightStrip.h"

LightStrip::LightStrip() :
		LightContainer(0,0,0)
{
}

LightStrip::LightStrip(int length, int stripOffset, int channel) :
		LightContainer(length,stripOffset,channel)
{
}
LightStrip::LightStrip(const LightStrip &other) :
		LightContainer(other)
{
}
/*void LightStrip::operator=(const LightStrip &other)
{
	Length = other.Length;
	Lights = other.Lights;
	Delay = other.Delay;
	StripOffset = other.StripOffset;
	Channel = other.Channel;
}*/
LightStrip::~LightStrip()
{
}

void LightStrip::Update(i64 now, DanceController *dance)
{
	for (int i = 0; i < Length; i++)
	{
		Lights[i] = dance->GetColour(now - Delay[i]);
	}
}

void LightStrip::UpdateDelays(Style style, double period, bool flipflop)
{
	for (int i = 0; i < Length; i++)
	{
		i64 delay = 0;
		
		//fade
		if (style == Style::Fade)
		{
			//delay = (i64)(period/100.0);
		}
		else if (style == Style::Streak)
		{
			delay = i*(i64)(period/100.0);
		}
		else if (style == Style::StreakFade)
		{
			delay = i*(i64)(period/100.0);
		}
		else if (style == Style::Pulse)
		{
			//delay = (i64)(period/100.0);
		}
		else if (style == Style::FlipFlop)
		{
			delay = flipflop ? (i64)(i*((period-period*0.6)*0.7)/Length) : 0;
		}
		
		Delay[i] = delay;
	}
}

