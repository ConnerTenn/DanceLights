
class LightStrip;

#ifndef _LIGHT_STRIP_H_
#define _LIGHT_STRIP_H_

#include "DanceController.h"


class LightStrip
{
public:
	
	int Length;
	int StripOffset;
	int Channel;
	DynamicArray<RGB> Lights;
	DynamicArray<i64> Delay;
	
	
	LightStrip();
	LightStrip(int length, int stripOffset, int channel);
	LightStrip(const LightStrip &other);
	void operator=(const LightStrip &other);
	~LightStrip();
	
	
	void Update(i64 now, DanceController *dance);
	void UpdateDelays(Style style, double period, bool flipflop);
	
	void Draw(int X, int Y, int Direction);

};

#endif
