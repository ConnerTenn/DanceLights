
#include "LightStrip.h"

LightStrip::LightStrip(int length) :
		Lights(length), Length(length)
{
	for (int i = 0; i < Length; i++)
	{
		Lights[i] = {0,0,0};
	}
}
LightStrip::LightStrip(const LightStrip &other) :
		Lights(other.Length), Length(other.Length)
{
	VaryOverLength = other.VaryOverLength;
	for (int i = 0; i < Length; i++)
	{
		Lights[i] = other.Lights.Values[i];
	}
}

void LightStrip::Update(u64 now, std::vector<Streak> *streakList)
{
	//u64 now = GetMilliseconds();
	/*RGB colour = ColourVal(0.0);
	
	double scale = BeatCycle((double)(now-style->Align), 0.25, 0.0, 1.0, 1.0, 1.0/((double)style->Speed));
	colour = ColourScale(colour, scale);
	
	for (int i = 0; i < Length; i++)
	{
		Lights[i] = colour;
	}
	
	return;*/
	
	//Pulse
	{
		/*double scale = BeatCycle((double)(now-style->Align), 0.25, 0.0, 1.0, 1.0, 1.0/((double)style->Period));
		RGB colour = ColourVal(style->Colour);
		colour = ColourScale(colour, scale);
		for (int i = 0; i < Length; i++)
		{
			Lights[i] = colour;
		}*/
	}
	//Streak
	{
		for (int i = 0; i < Length; i++)
		{
			double scale = 0;
			for (int j = 0; j < (int)streakList->size(); j++)
			{
				Streak *streak = &(*streakList)[j];
				//double temp = SingleBeat((now-streak->Offset-i*(streak->Speed/Length)), streak->Attack*(streak->Speed/Length), streak->Sustain*(streak->Speed/Length), streak->Attack*(streak->Speed/Length));
				double temp = ASD((double)(now-streak->Offset)-i*streak->Speed, streak->Attack, streak->Sustain, streak->Decay);
				scale = MAX(scale, temp);
			}
		
			RGB colour = ColourScale(ColourVal(0.0), scale);
			Lights[i] = colour;
		}
	}
	
	//Fade
	{
		/*for (int i = 0; i < Length; i++)
		{
			RGB colour = ColourVal(style->Colour);//+i*((double)Length)/((double)style->Period));
			Lights[i] = colour;
		}*/
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

