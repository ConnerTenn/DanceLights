

#include "DanceController.h"

class LightStrip
{
public:
	
	DynamicArray<RGB> Lights;
	int Length;
	bool Dynamic = true;
	
	LightStrip(int length);
	
	
	void Update(Style *style);
	
	void Draw(int X, int Y, int Direction);

};
