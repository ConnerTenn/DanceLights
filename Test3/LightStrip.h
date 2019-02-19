

#include "DanceController.h"

class LightStrip
{
public:
	
	DynamicArray<RGB> Lights;
	
	LightStrip(int length);
	
	void Draw(int X, int Y, int Direction);

};
