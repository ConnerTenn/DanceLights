

#include "DanceController.h"

class LightStrip
{
public:
	int Length;
	
	std::vector<RGB> Lights;
	
	void Draw(int X, int Y, int Direction);

};
