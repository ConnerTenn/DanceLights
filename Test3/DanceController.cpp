
#include "DanceController.h"

double Bistable(double x)
{
	return x > 1 ? 1 : (x < 0 ? 0 : (sin(PI*(x-0.5))+1.0)/2.0);
}

double ASD(double x, double a, double s, double d)
{
	//x = fmod(x, f);
	//return h*pow(M_E, -pow(a*sin(PI*x/f),2));
	return (a==0 ? (x>=0?1:0) : Bistable(x/a)) * (d==0 ? (x<=s+a && x>0 ? 1 : 0) : 1 - Bistable((x-a-s)/d));
}

double ASDR(double x, double a, double s, double d, double r, double f)
{
	double t = a+s+d+r;
	return ASD(fmod(f*x,1),a/t,s/t,d/t);
}

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
	if (Period == 0)
	{
		if (Align == 0) { Align = t; }
		Period = t - Align;
	}
	else 
	{
		u16 count = 0;
		u64 newFreq = 0; count = 0;
		for (int i = 0; i < PulseHist.Size()-1; i++)
		{
			if (PulseHist[i]-PulseHist[i+1]<MAX(1000,Period*5))// || t-LastAccept>=2000)
			{
				//frequency is based off of the overage of the current frequency and the timing of the last pulse
				newFreq += ((PulseHist[i]-PulseHist[i+1]) + Period)/2;
				LastAccept=t;
				count++;
			}
			else { i=PulseHist.Size(); }
		}
		if (count) { Period = newFreq / count; }
		
		u64 newOff = 0; count = 0;
		for (int i = 0; i < PulseHist.Size()-1; i++)
		{
			//if (PulseHist[i-1]-PulseHist[i]<Period*3)
			u64 q = (PulseHist[0]-Align-Period/2)/Period+1;
			if (i==0 || PulseHist[i]-PulseHist[i+1]<Period*2)
			{
				//Calculates the new align based on how far off the new pulse was
				//newOff += MOD(PulseHist[i]-Align-Period/2,Period)+Align-Period/2;
				newOff += (PulseHist[i]-Align-Period/2)+Align-Period/2-q*Period-i*Period;
				count++;
			}
			else { i=PulseHist.Size(); }
			//std::cout << "    Diff: " << (i64)((i64)PulseHist[i]-(i64)PulseHist[i+1]) << "    DiffDiff: " << ABS(((i64)PulseHist[i]-(i64)PulseHist[i+1])-(i64)Period) << "\n";
		}
		Align = newOff / count;
		//std::cout << "Count: " << count << "\n";
	}
	//std::cout << "Off: " << Align << "  Freq: " << Frequency << "\n";
}

bool Cycle::operator()(u64 t, u64 error, bool symmetricError, bool *latch)
{
	if (Period == 0) { return false; }
	if (MOD((i64)(t-Align+error/2*(u8)symmetricError),(i64)Period) < (i64)error)
	{
		if (!latch || !*latch)
		{
			if (latch) { *latch = true; }
			return true;
		}
		else 
		{
			return false;
		}
	}
	if (latch) { *latch = false; }
	return false;
}


DanceController::DanceController() : 
		UpdateCycle(), Beat(4), StateHist(120)
{
	Start = GetMilliseconds();
	UpdateCycle.Period = 1000.0/UpdateFreq;
	UpdateCycle.Align = StartTime;
	//Beat.Period = 500;
	//Beat.Align = StartTime;
	Beat.ActOnPulseOn = true;
	
	
	LightStripList.push_back(LightStrip(70));
	LightStripList.push_back(LightStrip(70));
	LightStripList.push_back(LightStrip(70));
	//Last=Start;
}

void DanceController::Update()
{
	Now = GetMilliseconds();
	
	//Delta = Now - Last;
	//if (Delta > (u64)(1000/UpdateFreq))
	static bool updateLatch = false;
	if (UpdateCycle(Now, UpdateCycle.Period/4, false, &updateLatch))
	{
		//u8 temp[5] = {StateIn[0],StateIn[1],StateIn[2],StateIn[3],BeatIn};
		StateHist.InsertBegin({StateIn[0],StateIn[1],StateIn[2],StateIn[3],BeatIn});
	}
	
	if (StateHist[0][4]) { Beat.PulseOn(Now); }
	else { Beat.PulseOff(Now); }
	
	static bool beatLatch = false;
	if (Beat(Now, UpdateCycle.Period/4, false, &beatLatch))
	{
		Streak streak;
		streak.Attack = 50.0;
		streak.Decay = 50.0;
		streak.Sustain = 0.0;
		streak.Offset = Now;
		static bool flip = false;
		streak.Speed = 200.0/70;
		if (flip)
		{
			streak.Attack = 10;
			streak.Decay = 200;
			streak.Sustain = 0;
			streak.Speed = 0;
			//StreakList.push_back(streak);
		}
		StreakList.push_back(streak);
		flip = !flip;
	}
	
	int length = 0;
	u64 delay = 0;
	for (u64 i = 0; i < LightStripList.size(); i++)
	{
		LightStripList[i].Update(Now - delay, &StreakList);
		length = MAX(length, LightStripList[i].Length);
		//delay += 500;
	}
	//delay-=500;
	
	for (u64 i = 0; i < StreakList.size();)
	{
		if (Now - StreakList[i].Offset > 5000)//StreakList[i].Speed+(u64)(2*StreakList[i].Attack*(StreakList[i].Speed/length) + StreakList[i].Sustain*(StreakList[i].Speed/length)) + delay)
		{
			StreakList.erase(StreakList.begin()+i);
		}
		else { i++; }
	}
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
		if (Beat(Now - i*UpdateCycle.Period, UpdateCycle.Period))
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
	
	DrawText(xOff+10, StateHist.Size()*10+20+yOff+35, "Beat Period:" + std::to_string(Beat.Period), {255,255,255});
	DrawText(xOff+200, yOff+80, "Streak Count:" + std::to_string(StreakList.size()), {255,255,255});
	
	for (int i = 0; i < (int)LightStripList.size(); i++)
	{
		for (int j = 0; j < LightStripList[i].Length; j++)
		{
			DrawRectangle(xOff + 200 + j*20, yOff + 100 + i*40, 20, 20, LightStripList[i].Lights[j]);
		}
		OutlineRectangle(xOff + 200, yOff + 100 + i*40, 20*LightStripList[i].Length, 20, {255,255,255});
	}
}