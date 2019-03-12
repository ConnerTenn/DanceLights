
#include "LightMatrix.h"

LightMatrix::LightMatrix() :
		LightContainer(0, 0, 0)
{
	/*StripOffset = 0;
	Channel = 0;*/
}

LightMatrix::LightMatrix(int width, int height, int stripOffset, int channel) :
		LightContainer(width*height,stripOffset,channel), Width(width), Height(height)
{
	/*StripOffset = stripOffset;
	Channel = channel;
	for (int i = 0; i < Length; i++)
	{
		Lights[i] = {0,0,0};
		//Delay[i] = (i64)(((1.0-cos(TAU*(i-Length/2)/30.0)+1)/2.0)*100.0);
		Delay[i] = i*50;
	}*/
}
LightMatrix::LightMatrix(const LightMatrix &other) :
		LightContainer(other)
{
	/*StripOffset = other.StripOffset;
	Channel = other.Channel;
	for (int i = 0; i < Length; i++)
	{
		Lights[i] = other.Lights.Values[i];
		Delay[i] = other.Delay.Values[i];
	}*/
}
LightMatrix::~LightMatrix()
{
	
}

void LightMatrix::Update(i64 now, DanceController *dance)
{
	for (int i = 0; i < Length; i++)
	{
		Lights[i] = dance->GetColour(now - Delay[i]);
	}
}

void LightMatrix::UpdateDelays(Style style, double period, bool flipflop)
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
			delay = (i/Width)*(i64)(period/100.0);
		}
		else if (style == Style::StreakFade)
		{
			delay = (i/Width)*(i64)(period/100.0);
		}
		else if (style == Style::Pulse)
		{
			//delay = (i64)(period/100.0);
		}
		else if (style == Style::FlipFlop)
		{
			delay = flipflop ? (i64)((i/Width)*((period-period*0.6)*0.7)/Height) : 0;
		}
		
		Delay[i] = delay;
	}
}
