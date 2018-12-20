
#include "Globals.h"
#include "DanceLight.h"

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

class DanceController
{
	std::vector<DanceLight> DanceLightList;
	
	
	
	BeatCycle PrimaryBeat;
	BeatCycle OffBeat;
	
	//Asynchronous
	void Input();
	
	void Update();
};
