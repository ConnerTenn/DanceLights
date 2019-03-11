
class LightContainer;

#ifndef _LIGHT_CONTAINER_H_
#define _LIGHT_CONTAINER_H_

#include "DanceController.h"


class LightContainer
{
public:
	int Length;
	int StripOffset;
	int Channel;
	int Xoff, Yoff;
	
	DynamicArray<RGB> Lights;
	DynamicArray<i64> Delay;
	
	LightContainer();
	LightContainer(int length, int stripOffset, int channel, int xoff, int yoff);
	LightContainer(const LightContainer &other);
	virtual ~LightContainer();
	
	virtual void Update(i64 now, DanceController *dance) = 0;
	virtual void UpdateDelays(Style style, double period, bool flipflop) = 0;
	
	virtual void Draw(int xoff, int yoff) = 0;

};

#endif
