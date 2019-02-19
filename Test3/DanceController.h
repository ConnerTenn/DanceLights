
#include "Globals.h"
#include "WindowController.h"

struct Style
{
	//Fade, hold, streak, 
	int Speed;
};

class DanceController
{
public:
	u8 State[4];
	u8 Beat;
	
	int UpdateFreq = 100;
	u64 Delta;
	
	RoundBuffer<Array<u8,5>> StateHist;
	
	DanceController();
	
	void Update();
	void Draw(int xOff, int yOff);
	
};

