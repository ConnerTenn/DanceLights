
#ifndef _DANCE_LIGHT_H_
#define _DANCE_LIGHT_H_

#include "Screen.h"

class LEDController
{
protected:
	
public:
	LEDController();
	
public:
	void Update();
	void Draw(void (* draw)());
};

class LEDSpectrum : LEDController
{
public:
	LEDSpectrum();
	
	void Update();
	void Draw(void (* draw)());
};

class LEDStrip : LEDController
{
	int Length;
	RGB *LEDs;
	
public:
	LEDStrip();
	LEDStrip(int length);
	LEDStrip(const LEDStrip &other);
	~LEDStrip();
	
	int GetLength();

	void Update();
	void Draw(void (* draw)(int, int, RGB));
	
};

class DanceLight 
{
	std::vector<LEDController> Controllers;
	
public:
	void Update();
	void Draw(void (* draw)());
};

#endif
