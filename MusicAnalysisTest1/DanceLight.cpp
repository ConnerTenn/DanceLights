
#include "DanceLight.h"

u8 RGB::operator[](int i)
{
	switch (i) 
	{
		case 0: return R;
		case 1: return G;
		case 2: return B;
		default: return 0;
	}
}


LEDController::LEDController()
{
	
}

void LEDController::Update()
{
	
}

void LEDController::Draw(void (* draw)())
{
	
}


LEDSpectrum::LEDSpectrum()
{

}


LEDStrip::LEDStrip() : LEDStrip(5) 
{

}

LEDStrip::LEDStrip(int length) : Length(length) 
{
	LEDs = new RGB[Length];
}

LEDStrip::LEDStrip(const LEDStrip &other)
{
	LEDs = other.LEDs;
	Length = other.Length;
	((LEDStrip *)&other)->LEDs = 0;
}

LEDStrip::~LEDStrip()
{
	if (LEDs) { delete[] LEDs; }
	LEDs = 0;
}

int LEDStrip::GetLength() { return Length; }

void LEDStrip::Update()
{
	for (int i = 0; i < Length; i++)
	{
		u64 t = GetMilliseconds();
		LEDs[i] = {(u8)(t), (u8)(t>>1), (u8)(t>>2)};
	}
}

void LEDStrip::Draw(void (* draw)(int x, int y, RGB colour))
{
	for (int i = 0; i < Length; i++)
	{
		draw(i,0,LEDs[i]);
	}
}


void DanceLight::Update()
{
	for (u64 i = 0; i < Controllers.size(); i++)
	{
		Controllers[i].Update();
	}
}

void DanceLight::Draw(void (* draw)())
{
	for (u64 i = 0; i < Controllers.size(); i++)
	{
		Controllers[i].Draw(draw);
	}
}
