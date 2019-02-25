
class LightStrip;

#ifndef _LIGHT_STRIP_H_
#define _LIGHT_STRIP_H_

#include "DanceController.h"

class LightStrip
{
public:
	
	DynamicArray<RGB> Lights;
	int Length;
	bool VaryOverLength = true;
	
	
	LightStrip(int length);
	LightStrip(const LightStrip &other);
	
	
	void Update(u64 now, std::vector<Streak> *streakList);//Style *style);
	
	void Draw(int X, int Y, int Direction);

};

#endif
