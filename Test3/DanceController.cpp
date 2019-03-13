
#include "DanceController.h"

/*double Bistable(double x, double a)
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
}*/

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
		for (int i = 0; i < PulseHist.size()-1; i++)
		{
			if (PulseHist[i]-PulseHist[i+1]<MAX(1000,Period*5))// || t-LastAccept>=2000)
			{
				//frequency is based off of the overage of the current frequency and the timing of the last pulse
				newFreq += ((PulseHist[i]-PulseHist[i+1]) + Period)/2.0;
				LastAccept=t;
				count++;
			}
			else { i=PulseHist.size(); }
		}
		if (count) { Period = newFreq / count; }
		
		i64 newAlign = 0; count = 0;
		for (int i = 0; i < PulseHist.size()-1; i++)
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
			else { i=PulseHist.size(); }
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
		UpdateCycle(), Beat(4), StateHist(120), LightStripList(2)
{
	Start = GetMilliseconds();
	UpdateCycle.Period = 20;
	UpdateCycle.Align = StartTime;
	//Beat.Align = StartTime;
	Beat.ActOnPulseOn = true;
	Beat.Period = 400;
	
	LightStripList[0]= new LightStrip(100,0,0,0,0);
	LightStripList[1]= new LightMatrix(38,7,0,0,0,40);
	//LightStripList.push_back(LightStrip(30,0,0));
	//LightStripList.push_back(LightStrip(30,30,0));
	//LightStripList.push_back(LightStrip(30,60,0));
	//LightStripList.push_back(LightStrip(30,90,0));
	//LightStripList.push_back(LightStrip(70,0,0));
	//LightStripList.push_back(LightStrip(110,0,0));
	//Last=Start;

	NextStyle = Style::Streak;
	MajorWeight = 1;
}
DanceController::~DanceController()
{
	for (int i = 0; i < LightStripList.size(); i++)
	{
		delete LightStripList[i];
		LightStripList[i] = 0;
	}
}

void DanceController::Update()
{
	Now = GetMilliseconds() - GlobalDelay;
	
	//Delta = Now - Last;
	//if (Delta > (u64)(1000/UpdateFreq))
	static bool updateLatch = false;
	if (UpdateCycle(Now, UpdateCycle.Period/2, false, &updateLatch))
	{
		//u8 temp[5] = {StateIn[0],StateIn[1],StateIn[2],StateIn[3],BeatIn};
		StateHist.InsertBegin({StateIn[0],StateIn[1],StateIn[2],StateIn[3],BeatIn});
		
		if (BeatIn) { Beat.PulseOn(Now); }
		else { Beat.PulseOff(Now); }
	}
	
	if (Speed == -1) { MulBeat.Period = Beat.Period*2; }
	else if (Speed == 1) { MulBeat.Period = Beat.Period/2; }
	else if (Speed == -2) { MulBeat.Period = Beat.Period*5; }
	else if (Speed == 2) { MulBeat.Period = Beat.Period/4; }
	else { MulBeat.Period = Beat.Period; }
	MulBeat.Align = Beat.Align;
	
	static bool beatLatch = false;
	static double lastPeriod = 0;
	bool updateDelays = false;
	if (!Hold && ((MulBeat(Now, UpdateCycle.Period/1.5, false, &beatLatch) && !Manual) || ForceUpdate))
	{
		if (CurrStyle != NextStyle)
		{
			CurrStyle = NextStyle;
			updateDelays = true;
		}
		
		if (lastPeriod != Beat.Period)
		{
			lastPeriod = Beat.Period;
			updateDelays = true;
		}
		
		if (CurrStyle == Style::FlipFlop) { updateDelays = true; }
		
		double attack = 0.3, decay = 0.5;
		if (MajorWeight == -1) { attack=0.5; decay=0.5; }
		if (MajorWeight == 1) { attack=0.1; decay=0.6; }
		
		ColourTimestamp timestamp;
		if (CurrStyle == Style::Fade)
		{
			timestamp.Time = Now;
			timestamp.Attack = MulBeat.Period*(attack+decay);
			timestamp.Colour = ColourPicker();
			ColourHist.push_back(timestamp);
		}
		if (CurrStyle == Style::Streak || (CurrStyle == Style::FlipFlop && !FlipFlop))
		{
			timestamp.Time = Now;
			timestamp.Attack = MulBeat.Period*attack;
			timestamp.Colour = ColourPicker();
			ColourHist.push_back(timestamp);
			timestamp.Time = Now+MulBeat.Period*attack;
			timestamp.Attack = MulBeat.Period*decay;
			timestamp.Colour = { 0,0,0 };
			ColourHist.push_back(timestamp);
			if (CurrStyle == Style::FlipFlop) { Oldest = Now; }
		}
		else if (CurrStyle == Style::StreakFade)
		{
			timestamp.Time = Now;
			timestamp.Attack = MulBeat.Period*attack;
			timestamp.Colour = ColourPicker();
			ColourHist.push_back(timestamp);
		}
		if (CurrStyle == Style::Pulse || (CurrStyle == Style::FlipFlop && FlipFlop))
		{
			timestamp.Time = Now;
			timestamp.Attack = MulBeat.Period*attack;
			timestamp.Colour = ColourPicker();
			ColourHist.push_back(timestamp);
			timestamp.Time = Now+MulBeat.Period*attack;
			timestamp.Attack = MulBeat.Period*decay;
			timestamp.Colour = { 0,0,0 };
			ColourHist.push_back(timestamp);
		}
		
		FlipFlop = !FlipFlop;
		ForceUpdate = false;	
	}
	
	static bool textLatch = false; 
	if ((Now-Start) % 12000 <= 100) { if (!textLatch) { ((LightMatrix *)LightStripList[1])->DrawText(Now, "OwO");} textLatch = true; } else { textLatch = false; }
	
	if (Hold)
	{
		if (HoldAlign == 0) { HoldAlign = Now; }
		//HoldAlign = Now;
		GlobalDelay = GetMilliseconds() - HoldAlign;
	}
	else 
	{
		HoldAlign = 0;
	}
	
	for (i64 i = 0; i < (i64)LightStripList.size(); i++)
	{
		if (updateDelays) { LightStripList[i]->UpdateDelays(CurrStyle, MulBeat.Period, FlipFlop); }
		LightStripList[i]->Update(Now/*-i*50*/, this);
	}
	
	for (i64 i = 0; i < (i64)ColourHist.size();)
	{
		if ((i64)Now - (i64)ColourHist[i].Time > 10'000)//StreakList[i].Speed+(u64)(2*StreakList[i].Attack*(StreakList[i].Speed/length) + StreakList[i].Sustain*(StreakList[i].Speed/length)) + delay)
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
	
	OutlineRectangle(90+xOff, yOff, 30, 10, RGB{255,255,255});
	//OutlineRectangle(90+xOff, 15+yOff, 30, 10, RGB{255,255,255});
	DrawRectangle(10*MajorWeight+100+xOff, yOff, 10, 10, RGB{255,255,255});
	//DrawRectangle(10*MinorWeight+100+xOff, 15+yOff, 10, 10, RGB{255,255,255});
	
	DrawText(130+xOff, 40+yOff, ((LightMatrix *)LightStripList[1])->Text, {255,255,255});
	
	
	//Draw StateIn Hist
	for (int i = 0; i < StateHist.size(); i++)
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
	OutlineRectangle(xOff+10-10, yOff+35, 20+4*10+20, 10*StateHist.size(), RGB{255,255,255});
	
	DrawText(xOff+10, StateHist.size()*10+20+yOff+35, "Beat Period:" + std::to_string(Beat.Period) + "   Beat Align:" + std::to_string(Beat.Align), {255,255,255});
	DrawText(xOff+10, StateHist.size()*10+20+yOff+55, "MulBeat Period:" + std::to_string(MulBeat.Period) + "   MulBeat Align:" + std::to_string(MulBeat.Align), {255,255,255});
	
	//Draw Light Strips
	DrawText(xOff+170, yOff+80, "Timestamp Count:" + std::to_string(ColourHist.size()), {255,255,255});
	for (int i = 0; i < (int)LightStripList.size(); i++)
	{
		LightStripList[i]->Draw(xOff+170, yOff+100);
		/*for (int j = 0; j < LightStripList[i]->Length; j++)
		{
			DrawRectangle(xOff + 170 + j*20, yOff + 100 + i*40, 20, 20, LightStripList[i]->Lights[j]);
		}
		OutlineRectangle(xOff + 170, yOff + 100 + i*40, 20*LightStripList[i]->Length, 20, {255,255,255});*/
	}
	
}

/*RGB DanceController::GetColour(i64 now)
{
	RGB colour = {0,0,0};
	//if (now < Oldest) { return colour; }
	
	int max=ColourHist.size();
	int i = max-1;
	for (; i>=0 && ColourHist[i].Time > now; i--) { }
	if (max)
	{
		double mix = Bistable(now-ColourHist[i].Time, ColourHist[i].Attack);
		colour = ColourMix(i == 0 ? colour : RGBVal(ColourHist[i-1].Colour), RGBVal(ColourHist[i].Colour), mix);
	}
	
	return colour;
}*/


RGB DanceController::ColourPicker()
{
	static ColourVal last = {0,0};
	ColourVal choice;
	
	/*double r = (rand()%10000)/10000.0;
	double w = (0.8*MinorWeight+1.0)/4.0;
	choice.Colour = (0.0<=r && r<0.25 ? 4.0*w*r : 
					(0.25<=r && r<0.75 ? (2.0-4.0*w)*r + 2.0*w-0.5 :
					(0.75<=r && r <= 1.0 ? 4.0*w*r+1-4.0*w : 0)));*/
	if (CurrStyle == Style::Fade)
	{
		const double vals[] = {-2.0/6.0,-1.0/6.0,1.0/6.0,2.0/6.0};
		choice.Colour = mmod(last.Colour + vals[rand()%4], 1.0);
	}
	else
	{
		choice.Colour = mmod(last.Colour + (rand()%5+1)/6.0, 1.0);
	}
	
	choice.Scale = 1.0;
	
	last = choice;
	return RGBScale(RGBVal(choice), 1);
}
