
#include "DanceController.h"

DanceController::DanceController() : 
		StateHist(32)
{
	
}

u64 LastTime = 0;
void DanceController::Update()
{
	u64 now = GetMilliseconds();
	Delta = now - LastTime;
	if (Delta > (u64)(1000/UpdateFreq))
	{
		//u8 temp[5] = {State[0],State[1],State[2],State[3],Beat};
		StateHist.InsertBegin({State[0],State[1],State[2],State[3],Beat});
		
		LastTime = now;
	}
}

void DanceController::Draw(int xOff, int yOff)
{
	for (int i = 0; i < 4; i++)
	{
		OutlineRectangle(15*i+xOff, yOff, 10, 10, RGB{255,255,255});
		if(State[i]) { DrawRectangle(15*i+xOff, yOff, 10, 10, RGB{255,255,255}); }
	}
	OutlineRectangle(xOff, yOff+15, 15*3+10, 10, RGB{255,255,255});
	if(Beat) { DrawRectangle(xOff, yOff+15, 15*3+10, 10, RGB{255,255,255}); }
	
	for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			OutlineRectangle(10*j+xOff, 10*i+yOff+35, 10, 10, RGB{255,255,255});
			if (StateHist[i][j]==1) { DrawRectangle(10*j+xOff, 10*i+yOff+35, 10, 10, RGB{255,255,255}); }
		}
	}
}