
class LightStrip;

#ifndef _LIGHT_STRIP_H_
#define _LIGHT_STRIP_H_

#include "DanceController.h"
#include "LightContainer.h"


class LightStrip : public LightContainer
{
public:
	
	LightStrip();
	LightStrip(int length, int stripOffset, int channel, int xoff, int yoff);
	LightStrip(const LightStrip &other);
	//void operator=(const LightStrip &other);
	~LightStrip();
	
	
	void Update(i64 now, DanceController *dance);
	void UpdateDelays(Style style, double period, bool flipflop);
	
	void Draw(int xoff, int yoff);

};

#endif
