
#include "LightStrip.h"

LightStrip::LightStrip(int length) :
		Lights(length), Delay(length), Length(length)
{
	for (int i = 0; i < Length; i++)
	{
		Lights[i] = {0,0,0};
	}
}
LightStrip::LightStrip(const LightStrip &other) :
		Lights(other.Length), Delay(other.Length), Length(other.Length)
{
	for (int i = 0; i < Length; i++)
	{
		Lights[i] = other.Lights.Values[i];
	}
}

void LightStrip::Update(u64 now, DanceController *dance)
{
	for (int i = 0; i < Length; i++)
	{
		Lights[i] = RGBVal(dance->GetColour(Delay[i]));
	}
}

void LightStrip::Draw(int xOff, int yOff, int direction)
{
	int x = xOff, y = yOff;
	for (int i = 0; i < Length; i++)
	{
		DrawRectangle(x, y, 20, 20, Lights[i]);
		if (direction == 0) {x+=20;}
		else if (direction == 1) {y+=20;}
	}
	if (direction == 0) { OutlineRectangle(xOff,yOff,Length*20,20,RGB{255,255,255}); }
	else if (direction == 1) { OutlineRectangle(xOff,yOff,20,Length*20,RGB{255,255,255}); }
}

