
#include "LightStrip.h"

void LightStrip::Draw(int X, int Y, int direction)
{
	int x = X, y = Y;
	for (int i = 0; i < Length; i++)
	{
		DrawRectangle(x, y, 10, 10, Lights[i]);
		if (direction == 0) {x+=10;}
		else if (direction == 1) {y+=10;}
	}
}

