
#include "DanceController.h"


void Cycle::PulseOn(u64 t)
{
	if (!PulseState)
	{
		OnTime = t;
		PulseState = true;
	}
}

void Cycle::PulseOff(u64 t)
{
	if (PulseState)
	{
		Pulse((t + OnTime)/2);
		PulseState = false;
	}
}

void Cycle::Pulse(u64 t)
{
	if (Frequency == 0)
	{
		if (Offset == 0) { Offset = t; }
		Frequency = t - Offset;
	}
	else 
	{
		//Offset = (MOD(t, Frequency)+Offset)/2;
		//Frequency = (MOD((i64)t-(i64)Offset-(i64)Frequency/2, (i64)Frequency)-(i64)Frequency/2)/2 + (i64)Frequency;
		if (t-LastPulse < Frequency*4)
		{
			Frequency = ((t-LastPulse) + Frequency)/2;
		}
		Offset = (2*MOD(t, Frequency)+Offset)/3;
	}
	std::cout << "  Off: " << Offset << "  Freq: " << Frequency << "\n";
	LastPulse = t;
}

bool Cycle::operator()(u64 t, u64 error)
{
	if (Frequency == 0) { return false; }
	if (MOD((i64)(t-Offset+error/2*(u8)SymmetricError),(i64)Frequency) < (i64)error)
	{
		if (!Triggered || !OncePerCycle)
		{
			Triggered = true;
			return true;
		}
		else 
		{
			return false;
		}
	}
	Triggered = false;
	return false;
}


DanceController::DanceController() : 
		StateHist(120)
{
	Start = GetMilliseconds();
	UpdateCycle.Frequency = 1000/UpdateFreq;
	UpdateCycle.Offset = StartTime;
	UpdateCycle.SymmetricError = false; UpdateCycle.OncePerCycle = true;
	Last=Start;
	//Beat.Frequency = 500;
	//Beat.Offset = StartTime;
}

void DanceController::Update()
{
	Now = GetMilliseconds();
	
	//Delta = Now - Last;
	//if (Delta > (u64)(1000/UpdateFreq))
	if (UpdateCycle(Now, UpdateCycle.Frequency/4))
	{
		//u8 temp[5] = {StateIn[0],StateIn[1],StateIn[2],StateIn[3],BeatIn};
		StateHist.InsertBegin({StateIn[0],StateIn[1],StateIn[2],StateIn[3],BeatIn});
	}
	
	if (StateHist[0][4]) { Beat.PulseOn(Now); }
	else { Beat.PulseOff(Now); }
}

void DanceController::Draw(int xOff, int yOff)
{
	//Draw Inputs
	for (int i = 0; i < 4; i++)
	{
		OutlineRectangle(15*i+xOff, yOff, 10, 10, RGB{255,255,255});
		if(StateIn[i]) { DrawRectangle(15*i+xOff, yOff, 10, 10, RGB{255,255,255}); }
	}
	OutlineRectangle(xOff, yOff+15, 15*3+10, 10, RGB{255,255,255});
	if(BeatIn) { DrawRectangle(xOff, yOff+15, 15*3+10, 10, RGB{255,255,255}); }
	
	//Draw StateIn Hist
	for (int i = 0; i < StateHist.Size(); i++)
	{
		if (Beat(Now - (i*1000/UpdateFreq+1000/UpdateFreq/2), UpdateCycle.Frequency))
		{
			DrawRectangle(xOff, i*10+yOff+35, 20+40, 10, RGB{255,0,0});
		}
		
		OutlineRectangle(xOff+20, 10*i+yOff+35, 40, 10, RGB{255,255,255});
		for (int j = 0; j < 4; j++)
		{
			if (StateHist[i][j]==1) { DrawRectangle(10*j+xOff+20, 10*i+yOff+35, 10, 10, RGB{255,255,255}); }
		}
		
		OutlineRectangle(xOff, 10*i+yOff+35, 20, 10, RGB{255,255,255});
		if(StateHist[i][4]) { DrawRectangle(xOff, 10*i+yOff+35, 20, 10, RGB{255,255,255}); }
	}
}