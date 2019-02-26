
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

void LightStrip::Update(u64 now, std::vector<Streak> *streakList, Fade *fade)
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
	/* 
	DynamicArray<double[2]> streakLights(Length);
	//Streak
	{
		for (int i = 0; i < Length; i++)
		{
			double scale = 0;
			double colourVal = 0;
			for (int j = 0; j < (int)streakList->size(); j++)
			{
				Streak *streak = &(*streakList)[j];
				//double temp = SingleBeat((now-streak->Offset-i*(streak->Speed/Length)), streak->Attack*(streak->Speed/Length), streak->Sustain*(streak->Speed/Length), streak->Attack*(streak->Speed/Length));
				double temp = ASD((double)(now-streak->Align)-i*streak->Speed, streak->Attack, streak->Sustain, streak->Decay);
				scale = MAX(scale, temp);
				colourVal = streak->Colour;
			}
		
			RGB colour = ColourScale(ColourVal(colourVal), scale);
			Lights[i] = colour;
			streakLights[i][0] = colourVal;
			streakLights[i][1] = scale;
		}
	}
	
	DynamicArray<double[2]> fadeLights(Length);
	//Fade
	{
		for (int i = 0; i < Length; i++)
		{
			RGB colour = ColourVal(fade->Colour);
			Lights[i] = colour;
			
			fadeLights[i][0] = fade->Colour;
		}
	}
	
	for (int i = 0; i < Length; i++)
	{
		//Lights[i] = ColourVal(RoundMean(streakLights[i][0], fadeLights[i][0], 1.0, streakLights[i][1]));
		double a = streakLights[i][0], b = fadeLights[i][0], w = streakLights[i][1];
		RGB rgbA = ColourVal(a); RGB rgbB = ColourScale(ColourVal(b), 0.5);
		Lights[i] = {(u8)(rgbA.R*w + rgbB.R*(1.0-w)), (u8)(rgbA.G*w + rgbB.G*(1.0-w)), (u8)(rgbA.B*w + rgbB.B*(1.0-w))};
		//Lights[i] = ColourVal(RED);
		//Lights[i] = ColourScale(Lights[i], streakLights[i][1]);
	}*/
	static DynamicArray<RGB> OldColour(Length);
	
	for (int i = 0; i < Length; i++)
	{
		double scale = 0;
		double colourVal = 0;
		for (int j = 0; j < (int)streakList->size(); j++)
		{
			Streak *streak = &(*streakList)[j];
			double temp = ASD((double)(now-streak->Align)-i*streak->Speed, streak->Attack, streak->Sustain, streak->Decay);
			scale = MAX(scale, temp);
			colourVal = streak->Colour;
		}
		
		
		Lights[i] = ColourMix(OldColour[i], ColourVal(colourVal), 1.0-scale);
		if (scale >= 0.8) { OldColour[i] = ColourVal(colourVal); }
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

