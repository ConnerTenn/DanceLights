
class LightStrip;

#ifndef _LIGHT_STRIP_H_
#define _LIGHT_STRIP_H_

#include "DanceController.h"

class LightStrip
{
public:
	
	int Length;
	DynamicArray<RGB> Lights;
	DynamicArray<i64> Delay;
	
	
	LightStrip(int length);
	LightStrip(const LightStrip &other);
	
	
	void Update(i64 now, DanceController *dance);//Style *style);
	void UpdateDelays(Style style, double period, bool flipflop);

};

#endif
