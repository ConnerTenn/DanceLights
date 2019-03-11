
#include "LightStrip.h"

LightStrip::LightStrip() :
		Length(0), Lights(0), Delay(0)
{
	StripOffset = 0;
	Channel = 0;
}

LightStrip::LightStrip(int length, int stripOffset, int channel) :
		Length(length), Lights(Length), Delay(Length)
{
	StripOffset = stripOffset;
	Channel = channel;
	for (int i = 0; i < Length; i++)
	{
		Lights[i] = {0,0,0};
		//Delay[i] = (i64)(((1.0-cos(TAU*(i-Length/2)/30.0)+1)/2.0)*100.0);
		Delay[i] = i*50;
	}
}
LightStrip::LightStrip(const LightStrip &other) :
		Length(other.Length), Lights(Length), Delay(Length)
{
	StripOffset = other.StripOffset;
	Channel = other.Channel;
	for (int i = 0; i < Length; i++)
	{
		Lights[i] = other.Lights.Values[i];
		Delay[i] = other.Delay.Values[i];
	}
}
void LightStrip::operator=(const LightStrip &other)
{
	Length = other.Length;
	Lights = other.Lights;
	Delay = other.Delay;
	StripOffset = other.StripOffset;
	Channel = other.Channel;
}
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

void LightStrip::Draw(int xOff, int yOff, int direction)
{
	int x = xOff, y = yOff;
	for (int i = 0; i < Length; i++)
	{
		DrawRectangle(x, y, 20, 20, Lights[i]);
		if (direction == 0) {x+=20;}
		else if (direction == 1) {y+=20;}
	}
	if (direction == 0) { OutlineRectangle(xOff,yOff,Length*20,20,RGB{255,255,255}); }
	else if (direction == 1) { OutlineRectangle(xOff,yOff,20,Length*20,RGB{255,255,255}); }
}

