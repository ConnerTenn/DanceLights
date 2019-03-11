
class LightMatrix;

#ifndef _LIGHT_MATRIX_H_
#define _LIGHT_MATRIX_H_


#include "DanceController.h"
#include "LightContainer.h"


class LightMatrix : public LightContainer
{
public:
	
	int Length;
	int StripOffset;
	int Channel;
	
	int Width;
	
	
	LightMatrix();
	LightMatrix(int length, int stripOffset, int channel);
	LightMatrix(const LightMatrix &other);
	void operator=(const LightMatrix &other);
	~LightMatrix();
	
	
	void Update(i64 now, DanceController *dance);
	void UpdateDelays(Style style, double period, bool flipflop);
	
	void Draw(int X, int Y, int Direction);

};

#endif
