
#include "LightMatrix.h"

LightMatrix::LightMatrix() :
		LightContainer(0, 0, 0, 0, 0)
{
	/*StripOffset = 0;
	Channel = 0;*/
}

LightMatrix::LightMatrix(int width, int height, int stripOffset, int channel, int xoff, int yoff) :
		LightContainer(width*height,stripOffset,channel,xoff,yoff), Width(width), Height(height)
{
	/*StripOffset = stripOffset;
	Channel = channel;
	for (int i = 0; i < Length; i++)
	{
		Lights[i] = {0,0,0};
		//Delay[i] = (i64)(((1.0-cos(TAU*(i-Length/2)/30.0)+1)/2.0)*100.0);
		Delay[i] = i*50;
	}*/
}
LightMatrix::LightMatrix(const LightMatrix &other) :
		LightContainer(other)
{
	/*StripOffset = other.StripOffset;
	Channel = other.Channel;
	for (int i = 0; i < Length; i++)
	{
		Lights[i] = other.Lights.Values[i];
		Delay[i] = other.Delay.Values[i];
	}*/
}
LightMatrix::~LightMatrix()
{
	
}

void LightMatrix::Update(i64 now, DanceController *dance)
{
	for (int i = 0; i < Length; i++)
	{
		Lights[i] = dance->GetColour(now - Delay[i]);
	}
	
	
	for (int t = 0; t < (int)Text.size(); t++)
	{
		int w = 0;
		bool draw = false;
		for (int i = 0; i < (int)Text[t].Text.size(); i++)
		{
			int j = (now-Text[t].StartTime)/100;
			char c = Text[t].Text[i];
			if (c>= 'a' && c <= 'z') { c = c-'a'; }
			else if (c>= 'A' && c <= 'Z') { c = c-'A'; }
			if (c>=0 && c <= (int)sizeof(Characters))
			{
				DrawCharacter(Width-j-1+w, c);
				w += Characters[(int)c][7]+1;
				if (Width-j-1+w >= 0) { draw = true; }
			}
		}
		if (!draw) { Text.erase(Text.begin()+t); t--; }
	}
}

void LightMatrix::UpdateDelays(Style style, double period, bool flipflop)
{
	for (int i = 0; i < Length; i++)
	{
		i64 delay = 0;
		
		//fade
		if (style == Style::Fade)
		{
			//delay = (i64)(period/100.0);
		}
		else if (style == Style::Streak)
		{
			delay = (i/Width)*(i64)(period/100.0);
		}
		else if (style == Style::StreakFade)
		{
			delay = (i/Width)*(i64)(period/100.0);
		}
		else if (style == Style::Pulse)
		{
			//delay = (i64)(period/100.0);
		}
		else if (style == Style::FlipFlop)
		{
			delay = flipflop ? (i64)((i/Width)*((period-period*0.6)*0.7)/Height) : 0;
		}
		
		Delay[i] = delay;
	}
}

void LightMatrix::SetPixel(int x, int y, RGB colour)
{
	int i = x+y*Width;
	if (x < 0 || x >= Width || y < 0 || y >= Height) { return; }
	Lights[i]=colour;
}

void LightMatrix::DrawCharacter(int xPos, char c)
{
	u8 w = Characters[(int)c][7];
	for (int y = 0; y < 7; y++)
	{
		for (int x = 0; x < w; x++)
		{
			//std::cout << "Pix " << xPos+x << " " << y << " " << (int)((Characters[(int)c][y]>>(x))&1) << "\n";
			if ((Characters[(int)c][y]>>(w-x-1))&1) { SetPixel(xPos+x, y, RGB{255,255,255}); }
			//SetPixel(xPos+x, y, RGB{255,255,255});
		}
	}
}

int LightMatrix::TextWidth(std::string text)
{
	int len = 0;
	for (int i = 0; i < (int)text.size(); i++)
	{
		char c = text[i];
		if (c>= 'a' && c <= 'z') { c = c-'a'; }
		else if (c>= 'A' && c <= 'Z') { c = c-'A'; }
		len+=Characters[(int)c][7];
	}
	return len;
}

void LightMatrix::DrawText(i64 now, std::string text)
{
	int len = 0; 
	int j = 0;
	if (Text.size()) 
	{ 
		len = TextWidth(Text.back().Text); 
		j = (now - Text.back().StartTime)/100;
		
		std::cout << "len " << len << " j " << j << "\n";
		if (Width-j-1 + len < Width) 
		{
			Text.push_back( TextTimestamp{text, now} );
		}
		else
		{
			Text.back().Text += text;
		}
	}
	else
	{
		Text.push_back( TextTimestamp{text, now} );
	}
	
}

void LightMatrix::Draw(int xoff, int yoff)
{
	//int x = xOff, y = yOff;
	for (int i = 0; i < Length; i++)
	{
		DrawRectangle((i%Width)*20+Xoff+xoff, (i/Width)*20+Yoff+yoff, 20, 20, Lights[i]);
	}
	OutlineRectangle(Xoff+xoff,Yoff+yoff,Width*20,Height*20,RGB{255,255,255});
}

