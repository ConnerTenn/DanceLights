
#include "LightMatrix.h"

int MatrixLengths[][3] =
{
	{37, 0, 0},
	{38, 0, 0},
	{38, 0, 0},
	{39, 0, 0},
	{39, 0, 0},
	{39, 0, 0},
	{40, 0, 1},
	{40, 0, 1},
	{42, 0, 1},
};

LightMatrix::LightMatrix() :
		LightContainer(0, 0, 0,0)
{
}

LightMatrix::LightMatrix(int channel) :
		LightContainer(352, 0, 0, channel)
{
	Height = 9;
	for (int i = 0, t = 0; i < Height; i++)
	{
		MatrixLengths[i][1] = t;
		t+=MatrixLengths[i][0];
		Width = MAX(MatrixLengths[i][0], Width);
	}
}

LightMatrix::LightMatrix(int width, int height, int stripOffset, int timeOffset, int channel, bool inverse) :
		LightContainer(width*height,stripOffset,timeOffset,channel,inverse), Width(width), Height(height)
{
}
LightMatrix::LightMatrix(const LightMatrix &other) :
		LightContainer(other)
{
}
LightMatrix::~LightMatrix()
{
	
}

u8 ConvChar(u8 c)
{
	if (c < 26) {}
	else if (c>= 'a' && c <= 'z') { c = c-'a'; }
	else if (c>= 'A' && c <= 'Z') { c = c-'A'; }
	else { return -1; }
	
	return c;
}

int TextWidth(std::string text)
{
	int len = 0;
	for (int i = 0; i < (int)text.size(); i++)
	{
		len+=Characters[(int)ConvChar(text[i])][9-1];
	}
	return len;
}

void LightMatrix::Update(i64 now, DanceController *dance)
{
	for (int i = 0; i < Length; i++)
	{
		Lights[i] = dance->GetColour(now - Delay[i]);
		//std::cout << i << " " << (int)Lights[i].R << " " << (int)Lights[i].G << " " << (int)Lights[i].B << "   "<< Delay[i]<< "\n";
	}

	
	for (int t = 0; t < (int)Text.size(); t++)
	{
		int x = 0;
		bool draw = false;
		for (int i = 0; i < (int)Text[t].Text.size(); i++)
		{
			int j = (now-Text[t].StartTime)/100;
			char c = ConvChar(Text[t].Text[i]);
			int w = Characters[(int)c][9-1];
			if (c>=0 && c <= (int)sizeof(Characters) && Width-j-1+x < Width )
			{
				DrawCharacter(Width-j-1+x, c);
				//std::cout << "Draw " << (int)c << "\n";
				x += w+1;
				if (Width-j-1+x >= 0) { draw = true; }
			}
		}
		if (!draw) { Text.erase(Text.begin()+t); t--; }
	}
}

void LightMatrix::UpdateDelays(Style style, double period, bool flipflop)
{
	for (int y = 0; y < Height; y++)
	{
		int width = MatrixLengths[y][0];

		//std::cout << "\n<" << y << "> Width:" << width << "\n";
		for (int x = 0; x < width; x++)
		{
			i64 delay = TimeOffset;
		
			//fade
			if (style == Style::Fade)
			{
				//delay += (i64)(period/100.0);
			}
			else if (style == Style::Streak)
			{
				delay += (Height-y-1)*(i64)(period/100.0);
			}
			else if (style == Style::StreakFade)
			{
				delay += (Height-y-1)*(i64)(period/100.0);
			}
			else if (style == Style::Pulse)
			{
				//delay += (i64)(period/100.0);
			}
			else if (style == Style::FlipFlop)
			{
				delay += flipflop ? (i64)((Height-y-1)*((period-period*0.6)*0.7)/Height) : 0;
			}
			
			Delay[MatrixLengths[y][1]+x] = delay;
			//std::cout<<" i:"<<MatrixLengths[y][1]+x;
		}
	}
	/*
	for (int i = 0; i < Length; i++)
	{
		i64 delay = TimeOffset;
		
		//fade
		if (style == Style::Fade)
		{
			//delay += (i64)(period/100.0);
		}
		else if (style == Style::Streak)
		{
			delay += (i/Width)*(i64)(period/100.0);
		}
		else if (style == Style::StreakFade)
		{
			delay += (i/Width)*(i64)(period/100.0);
		}
		else if (style == Style::Pulse)
		{
			//delay += (i64)(period/100.0);
		}
		else if (style == Style::FlipFlop)
		{
			delay += flipflop ? (i64)((i/Width)*((period-period*0.6)*0.7)/Height) : 0;
		}
		
		Delay[i] = delay;
	}*/
}

void LightMatrix::SetPixel(int x, int y, RGB colour)
{
	if (x < 0 || x >= MatrixLengths[y][0] || y < 0 || y >= Height) { return; }
	Lights[x+MatrixLengths[y][1]]=colour;
}

void LightMatrix::DrawCharacter(int xPos, char c)
{
	u8 w = Characters[(int)c][9-1];
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < w; x++)
		{
			//std::cout << "(" << x << " " << y << ") ";
			if ((Characters[(int)c][y]>>(w-x-1)) & 1) { SetPixel(xPos+x+MatrixLengths[y][2], y, RGB{255,255,255}); /*std::cout << "1";*/ }
			//else {std::cout<<"0";}
		}
		//std::cout << " ";
	}
	//std::cout << "\n";
}

void LightMatrix::DrawText(i64 now, std::string text)
{
	int len = 0; 
	int j = 0;
	if (Text.size()) 
	{ 
		len = TextWidth(Text.back().Text); 
		j = (now - Text.back().StartTime)/100;
		
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


