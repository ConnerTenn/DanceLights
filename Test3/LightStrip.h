
class LightStrip;

#ifndef _LIGHT_STRIP_H_
#define _LIGHT_STRIP_H_

#include "DanceController.h"

class LightStrip
{
public:
	
	DynamicArray<RGB> Lights;
	DynamicArray<double> Delay;
	int Length;
	
	
	LightStrip(int length);
	LightStrip(const LightStrip &other);
	
	
	void Update(u64 now);//Style *style);
	
	void Draw(int X, int Y, int Direction);

};

#endif
