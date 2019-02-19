
#include "DanceController.h"

Cycle::Cycle(int len) : 
		PulseHist(len+1)
{
	
}

void Cycle::PulseOn(u64 t)
{
	if (!PulseState)
	{
		OnTime = t;
		PulseState = true;
		if (ActOnPulseOn) { Pulse(t); }
	}
}

void Cycle::PulseOff(u64 t)
{
	if (PulseState)
	{
		PulseState = false;
		if (!ActOnPulseOn) { Pulse((t + OnTime)/2); }
	}
}

void Cycle::Pulse(u64 t)
{
	PulseHist.InsertBegin(t);
	if (Frequency == 0)
	{
		if (Offset == 0) { Offset = t; }
		Frequency = t - Offset;
	}
	else 
	{
		/*if (t-LastPulse < Frequency*5)
		{
			//frequency is based off of the overage of the current frequency and the timing of the last pulse
			Frequency = ((t-LastPulse) + Frequency)/2;
		}
		//Calculates the new offset based on how far off the new pulse was
		Offset = MOD(t-Offset-Frequency/2,Frequency)+Offset-Frequency/2;*/
		//u64 oldFreq = Frequency;
		u64 newFreq = 0; u16 count = 0;
		for (int i = 0; i < PulseHist.Size()-1; i++)
		{
			if (PulseHist[i]-PulseHist[i+1]<Frequency*3 || t-LastAccept>=2000)
			{
				//frequency is based off of the overage of the current frequency and the timing of the last pulse
				newFreq += ((PulseHist[i]-PulseHist[i+1]) + Frequency)/2;
				LastAccept=t;
				count++;
			}
			else { i=PulseHist.Size(); }
		}
		if (count) { Frequency = newFreq / count; }
		
		u64 newOff = 0; count = 0;
		for (int i = 0; i < PulseHist.Size()-1; i++)
		{
			//if (PulseHist[i-1]-PulseHist[i]<Frequency*3)
			u64 q = (PulseHist[0]-Offset-Frequency/2)/Frequency+1;
			if (i==0 || PulseHist[i]-PulseHist[i+1]<Frequency*2)
			{
				//newOff += MOD(PulseHist[i]-Offset-Frequency/2,Frequency)+Offset-Frequency/2;
				newOff += (PulseHist[i]-Offset-Frequency/2)+Offset-Frequency/2-q*Frequency-i*Frequency;
				count++;
			}
			else { i=PulseHist.Size(); }
			//std::cout << "    Diff: " << (i64)((i64)PulseHist[i]-(i64)PulseHist[i+1]) << "    DiffDiff: " << ABS(((i64)PulseHist[i]-(i64)PulseHist[i+1])-(i64)Frequency) << "\n";
		}
		Offset = newOff / count;
		std::cout << "Count: " << count << "\n";
	}
	std::cout << "Off: " << Offset << "  Freq: " << Frequency << "\n";
	//LastPulse = t;
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
		UpdateCycle(), Beat(4), StateHist(120)
{
	Start = GetMilliseconds();
	UpdateCycle.Frequency = 1000/UpdateFreq;
	UpdateCycle.Offset = StartTime;
	UpdateCycle.SymmetricError = false; UpdateCycle.OncePerCycle = true;
	//Beat.Frequency = 500;
	//Beat.Offset = StartTime;
	Beat.ActOnPulseOn = true;
	
	//Last=Start;
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
		//Calculated Beat Cycle
		if (Beat(Now - (i*1000/UpdateFreq+1000/UpdateFreq/2), UpdateCycle.Frequency))
		{
			DrawRectangle(xOff, i*10+yOff+35, 20+40+20, 10, RGB{255,0,0});
		}
		
		//State Hist
		OutlineRectangle(xOff+20+10, 10*i+yOff+35, 40, 10, RGB{255,255,255});
		for (int j = 0; j < 4; j++)
		{
			if (StateHist[i][j]==1) { DrawRectangle(10*j+xOff+20+10, 10*i+yOff+35, 10, 10, RGB{255,255,255}); }
		}
		
		//Beat Hist
		OutlineRectangle(xOff+10, 10*i+yOff+35, 20, 10, RGB{255,255,255});
		if(StateHist[i][4]) { DrawRectangle(xOff+10, 10*i+yOff+35, 20, 10, RGB{255,255,255}); }
	}
}