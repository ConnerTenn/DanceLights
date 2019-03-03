
#include "DanceController.h"

double Bistable(double x)
{
	return x > 1 ? 1 : (x < 0 ? 0 : (sin(PI*(x-0.5))+1.0)/2.0);
}

double Bistable(double x, double a)
{
	return x<0?0 : (x<a && a!=0 ? x/a : 1);
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

double RoundMean(double a, double b, double m, double w)
{
	//return fmod((a+b+(abs(a-b)>m/2.0?m:0))/2.0, m);
	return fmod((a + (abs(a-b)>m/2.0?1.0:0.0))*w + b*(1.0-w),m);
}

i8 RoundDirection(double a, double b, double m)
{
	return a==b? 0 : (mmod(b-a,m)<mmod(a-b,m) ? 1 : -1);
}

Cycle::Cycle(int len) : 
		PulseHist(len+1)
{
	
}

void Cycle::PulseOn(i64 t)
{
	if (!PulseState)
	{
		OnTime = t;
		PulseState = true;
		if (ActOnPulseOn) { Pulse(t); }
	}
}

void Cycle::PulseOff(i64 t)
{
	if (PulseState)
	{
		PulseState = false;
		if (!ActOnPulseOn) { Pulse((t + OnTime)/2); }
	}
}

void Cycle::Pulse(i64 t)
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
		double newFreq = 0; count = 0;
		for (int i = 0; i < PulseHist.Size()-1; i++)
		{
			if (PulseHist[i]-PulseHist[i+1]<MAX(1000,Period*5))// || t-LastAccept>=2000)
			{
				//frequency is based off of the overage of the current frequency and the timing of the last pulse
				newFreq += ((PulseHist[i]-PulseHist[i+1]) + Period)/2.0;
				LastAccept=t;
				count++;
			}
			else { i=PulseHist.Size(); }
		}
		if (count) { Period = newFreq / count; }
		
		i64 newAlign = 0; count = 0;
		for (int i = 0; i < PulseHist.Size()-1; i++)
		{
			//if (PulseHist[i-1]-PulseHist[i]<Period*3)
			i64 q = (PulseHist[0]-Align-Period/2.0)/Period+1.0;
			if (i==0 || (double)(PulseHist[i]-PulseHist[i+1])<Period*2.0)
			{
				//Calculates the new align based on how far off the new pulse was
				//newOff += MOD(PulseHist[i]-Align-Period/2,Period)+Align-Period/2;
				newAlign += (PulseHist[i]-Align-Period/2)+Align-Period/2-q*Period-i*Period;
				count++;
			}
			else { i=PulseHist.Size(); }
			//std::cout << "    Diff: " << (i64)((i64)PulseHist[i]-(i64)PulseHist[i+1]) << "    DiffDiff: " << ABS(((i64)PulseHist[i]-(i64)PulseHist[i+1])-(i64)Period) << "\n";
		}
		Align = newAlign / count;
		//std::cout << "Count: " << count << "\n";
	}
	//std::cout << "Off: " << Align << "  Freq: " << Frequency << "\n";
}

bool Cycle::operator()(i64 t, double error, bool symmetricError, bool *latch)
{
	if (Period == 0) { return false; }
	if (mmod((double)(t-Align)+error/2.0*(symmetricError?1:0),Period) < (double)error)
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
	UpdateCycle.Period = 20;
	UpdateCycle.Align = StartTime;
	//Beat.Align = StartTime;
	Beat.ActOnPulseOn = true;
	
	
	LightStripList.push_back(LightStrip(70));
	LightStripList.push_back(LightStrip(70));
	LightStripList.push_back(LightStrip(70));
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
	
	if (HalfTime) { MulBeat.Period = Beat.Period*2; }
	else if (DoubleTime) { MulBeat.Period = Beat.Period/2; }
	else { MulBeat.Period = 0; }
	MulBeat.Align = Beat.Align;
	
	static bool beatLatch = false;
	if (Beat(Now, UpdateCycle.Period/4, false, &beatLatch))
	{
		static ColourTimestamp timestamp;
		timestamp.Time = Now;
		timestamp.Attack = 200;
		timestamp.Colour = { mmod(timestamp.Colour.Colour + (rand()%5+1)/6.0, 1.0), 1.0 };
		ColourHist.push_back(timestamp);
		//PrimaryColour = mmod(PrimaryColour + (rand()%3)/6.0+2.0/6.0, 1.0);
	}
	
	for (i64 i = 0; i < (i64)LightStripList.size(); i++)
	{
		LightStripList[i].Update(Now-i*20, this);
	}
	
	for (i64 i = 0; i < (i64)ColourHist.size();)
	{
		if ((i64)Now - (i64)ColourHist[i].Time > 5000)//StreakList[i].Speed+(u64)(2*StreakList[i].Attack*(StreakList[i].Speed/length) + StreakList[i].Sustain*(StreakList[i].Speed/length)) + delay)
		{
			ColourHist.erase(ColourHist.begin()+i);
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
		if (MulBeat(Now - i*UpdateCycle.Period, UpdateCycle.Period))
		{
			DrawRectangle(xOff, i*10+yOff+35+5/2, 20+40+20, 5, RGB{0,0,255});
		}
		
		//State Hist
		//OutlineRectangle(xOff+20+10, 10*i+yOff+35, 40, 10, RGB{255,255,255});
		for (int j = 0; j < 4; j++)
		{
			if (StateHist[i][j]==1) { DrawRectangle(10*j+xOff+20+10, 10*i+yOff+35, 10, 10, RGB{255,255,255}); }
		}
		
		//Beat Hist
		//OutlineRectangle(xOff+10, 10*i+yOff+35, 20, 10, RGB{255,255,255});
		if(StateHist[i][4]) { DrawRectangle(xOff+10, 10*i+yOff+35, 20, 10, RGB{255,255,255}); }
	}
	OutlineRectangle(xOff+10-10, yOff+35, 20+4*10+20, 10*StateHist.Size(), RGB{255,255,255});
	
	DrawText(xOff+10, StateHist.Size()*10+20+yOff+35, "Beat Period:" + std::to_string(Beat.Period) + "   Beat Align:" + std::to_string(Beat.Align), {255,255,255});
	DrawText(xOff+10, StateHist.Size()*10+20+yOff+55, "MulBeat Period:" + std::to_string(MulBeat.Period) + "   MulBeat Align:" + std::to_string(MulBeat.Align), {255,255,255});
	
	//Draw Light Strips
	DrawText(xOff+200, yOff+80, "Timestamp Count:" + std::to_string(ColourHist.size()), {255,255,255});
	for (int i = 0; i < (int)LightStripList.size(); i++)
	{
		for (int j = 0; j < LightStripList[i].Length; j++)
		{
			DrawRectangle(xOff + 200 + j*20, yOff + 100 + i*40, 20, 20, LightStripList[i].Lights[j]);
		}
		OutlineRectangle(xOff + 200, yOff + 100 + i*40, 20*LightStripList[i].Length, 20, {255,255,255});
	}
	
}

RGB DanceController::GetColour(i64 now)
{
	RGB colour = {0,0,0};
	//for (int i = ColourHist.size(); i >= 0; i--)
	int i = 0;
	//for (; i < (int)ColourHist.size()-1 && ColourHist[i+1].Time < now; i++)
	for (i=ColourHist.size()-1; i>=0 && ColourHist[i].Time > now; i--) { }
	if (ColourHist.size())
	{
		double mix = Bistable(now-ColourHist[i].Time, ColourHist[i].Attack);
		colour = ColourMix(i == 0 ? colour : RGBVal(ColourHist[i-1].Colour), RGBVal(ColourHist[i].Colour), mix);
	}
	
	/*for (int i = (int)ColourHist.size()-1; i>=0; i--)
	{
		if (i) { colour = RGBVal(ColourHist[i-1].Colour); } else { colour = {0,0,0}; }
		if (ColourHist[i].Time < now)
		{
			double mix = Bistable(now-ColourHist[i].Time, ColourHist[i].Attack);
			colour = ColourMix(colour, RGBVal(ColourHist[i].Colour), mix);
			return colour;
		}
	}*/
	/*if (now < 0) { now = 0; }
	if (now > 3000) { return {0,0,0}; }
	colour = ColourHistMap[now];*/
	return colour;
}

