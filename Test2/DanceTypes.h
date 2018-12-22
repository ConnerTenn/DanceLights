
#ifndef _DANCE_TYPES_H_
#define _DANCE_TYPES_H_

#include "Globals.h"

class BeatCycle
{
public:
	double Frequency;
	double Scale;
	double Attack;
	//double Offset;
	
	double operator[](double x)
	{
		return Scale*exp(-(Attack*pow(sin(Frequency*PI*x),2.0)));
	}
};

class Melody
{
public:
	bool Keys[88];
	double Attack;
	double Sustain;
	double Decay;
};


/*
template<class T>
class TimestampData
{
	u64 Time;
	T Data;
	
	TimestampData(T data)
	{
		Data = data;
		Time = GetMicroseconds();
	}
};
*/

#endif
