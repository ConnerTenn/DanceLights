
#include "DanceController.h"
#include "LightContainer.h"

LightContainer::LightContainer() :
		Length(0), Lights(0), Delay(0)
{
	StripOffset = 0;
	Channel = 0;
}

LightContainer::LightContainer(int length, int stripOffset, int timeOffset, int channel, bool inverse, bool constant) :
		Length(length), Inverse(inverse), Constant(constant), Lights(Length), Delay(Length)
{
	StripOffset = stripOffset;
	TimeOffset = timeOffset;
	Channel = channel;
	for (int i = 0; i < Length; i++)
	{
		Lights[i] = {0,0,0};
		//Delay[i] = (i64)(((1.0-cos(TAU*(i-Length/2)/30.0)+1)/2.0)*100.0);
		Delay[i] = i*50 + TimeOffset;
	}
}
LightContainer::LightContainer(const LightContainer &other) :
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


LightContainer::~LightContainer()
{
	
}

