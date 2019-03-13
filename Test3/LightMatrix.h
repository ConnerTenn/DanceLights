
class LightMatrix;

#ifndef _LIGHT_MATRIX_H_
#define _LIGHT_MATRIX_H_


#include "DanceController.h"
#include "LightContainer.h"

const u8 Characters[][8] = {
{0b0110, 0b1001, 0b1001, 0b1111, 0b1001, 0b1001, 0b1001, 4}, //A
{0b1110, 0b1001, 0b1001, 0b1110, 0b1001, 0b1001, 0b1110, 4}, //B
{0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 4}, //C
{0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 4}, //D
{0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 4}, //E
{0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 4}, //F
{0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 4}, //G
{0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 4}, //H
{0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 4}, //I
{0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 4}, //J
{0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 4}, //K
{0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 4}, //L
{0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 4}, //M
{0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 4}, //N
{0b0110, 0b1001, 0b1001, 0b1001, 0b1001, 0b1001, 0b0110, 4}, //O
{0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 4}, //P
{0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 4}, //Q
{0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 4}, //R
{0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 4}, //S
{0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 4}, //T
{0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 4}, //U
{0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 4}, //V
{0b0000, 0b0000, 0b0000, 0b0000, 0b10101, 0b10101, 0b01010, 5}, //w
{0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 4}, //X
{0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 4}, //Y
{0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 4}, //Z
};

class LightMatrix : public LightContainer
{
public:
	int Width, Height;
	std::string Text;
	i64 TextStartTime = 0;
	
	
	LightMatrix();
	LightMatrix(int width, int height, int stripOffset, int channel, int xoff, int yoff);
	LightMatrix(const LightMatrix &other);
	//void operator=(const LightMatrix &other);
	~LightMatrix();
	
	
	void Update(i64 now, DanceController *dance);
	void UpdateDelays(Style style, double period, bool flipflop);
	void SetPixel(int x, int y, RGB colour);
	void DrawCharacter(int xPos, char c);
	void DrawText(i64 now, std::string text);
	
	void Draw(int xoff, int yoff);

};

#endif
