
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

double Bistable(double x)
{
	return x > 1 ? 1 : (x < 0 ? 0 : (sin(PI*(x-0.5))+1.0)/2.0);
}

double SingleBeat(double x, double a, double s, double d)
{
	//x = fmod(x, f);
	//return h*pow(M_E, -pow(a*sin(PI*x/f),2));
	return (a==0 ? 0 : Bistable(x/a)) * (d==0 ? 1 : 1 - Bistable((x-a-s)/d));
}

double BeatCycle(double x, double a, double s, double d, double r, double f)
{
	double t = a+s+d+r;
	return SingleBeat(fmod(f*x,1),a/t,s/t,d/t);
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
				double temp = SingleBeat((now-streak->Offset-i*(streak->Speed/Length)), streak->Attack*(streak->Speed/Length), streak->Sustain*(streak->Speed/Length), streak->Attack*(streak->Speed/Length));
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

