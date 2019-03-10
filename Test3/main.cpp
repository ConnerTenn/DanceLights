
#include "Globals.h"
#include "WindowController.h"
#include "DanceController.h"
#include "LightStrip.h"

u64 StartTime = GetMicroseconds();
//TerminalBuffer TermBuffer(10, 900, 30);

DanceController Dance;
	
int main()
{
	InitWindow();
	srand(StartTime);
	
	//u64 startTime = GetMilliseconds();
	u64 lastTime = StartTime;
	u64 maxTime = 0;
	
	bool run = true;
	while (run)
	{		
		u64 key;
		XEvent event;
		if (PendingEvent(&event))
		{
			if (event.type == KeyPress) 
			{ 
				key = GetKeyPressed(&event); 
				
				if (key == 'u') { Dance.StateIn[0] = 1; }
				if (key == 'i') { Dance.StateIn[1] = 1; }
				if (key == 'o') { Dance.StateIn[2] = 1; }
				if (key == 'p') { Dance.StateIn[3] = 1; }
				if (key == ' ') { Dance.BeatIn = 1; }
				
				if (key == 'q') { Dance.Beat.Period = 0; Dance.Beat.Align = 0; }
				
				if (key == 'w') { Dance.Speed = -2; }
				if (key == 'e') { Dance.Speed = -1; }
				if (key == 'r') { Dance.Speed = 0; }
				if (key == 't') { Dance.Speed = 1; }
				if (key == 'y') { Dance.Speed = 2; }
				
				if (key == 'b') { Dance.MajorWeight = -1; }
				if (key == 'n') { Dance.MajorWeight = 0; }
				if (key == 'm') { Dance.MajorWeight = 1; }
				
				if (key == 'h') {Dance.NextStyle = Style::Pulse; }
				if (key == 'j') {Dance.NextStyle = Style::StreakFade; }
				if (key == 'k') {Dance.NextStyle = Style::Streak; }
				if (key == 'l') {Dance.NextStyle = Style::Fade; }
				if (key == 'g') {Dance.NextStyle = Style::FlipFlop; }
				
				if (key == 'a') { Dance.Hold = true; }
				if (key == 's') { Dance.Manual = true; }
				if (key == 'd') { Dance.ForceUpdate = true; }
				
				if (key == 65307) { run = false; }
			}
			else if (event.type == KeyRelease) 
			{ 
				key = GetKeyReleased(&event); 
				
				if (key == 'u') { Dance.StateIn[0] = 0; }
				if (key == 'i') { Dance.StateIn[1] = 0; }
				if (key == 'o') { Dance.StateIn[2] = 0; }
				if (key == 'p') { Dance.StateIn[3] = 0; }
				if (key == ' ') { Dance.BeatIn = 0; }
				
				/*if (key == 'w' && Dance.Speed == -2) { Dance.Speed = 0; }
				if (key == 'e' && Dance.Speed == -1) { Dance.Speed = 0; }
				if (key == 'r' && Dance.Speed == 1) { Dance.Speed = 0; }*/
				
				if (key == 'a') { Dance.Hold = false; }
				if (key == 's') { Dance.Manual = false; }
			}
		}
		
		Dance.Update();
		
		
		
		ForceClear();
		
		for (int i = 0; i < 100; i++)
		{
			DrawRectangle(400+10*i, 0, 20, 20, RGBVal({i/100.0,1.0}));
		}
		for (int i = 0; false && i < 100; i++)
		{
			int x = 2000, y = 300;
			double a = i/100.0, b = fmod(3.0*i/100.0,1.0);
			DrawRectangle(x, y+i*10, 10, 10, RGBVal({a,1}));
			DrawRectangle(x+10*6, y+i*10, 10, 10, RGBVal({b,1}));
			for (int j = 0; j < 5; j++)
			{
				double w = j/5.0;
				//double colour = RoundMean(a, b, 1.0, 1.0-j/5.0);
				RGB rgbA = RGBVal({a,1}); RGB rgbB = RGBVal({b,1});
				RGB rgb = ColourMix(rgbA, rgbB, w);
				DrawRectangle(x+j*10+10, y+i*10, 10, 10, rgb);
			}
		}
		{
			int x = 2000, y = 200;
			for (int i = 0; false && i < 200; i++)
			{
				//DrawPixel(50.0*sin(TAU*i/500.0)+x, 50.0*cos(TAU*i/500.0)+y, ColourVal(i/500.0));
				DrawLine(x,y,50.0*sin(TAU*i/200.0)+x, -50.0*cos(TAU*i/200.0)+y,RGBVal({i/200.0,1}));
			}
			//DrawLine(x,y,60.0*sin(TAU*ColourFade.Colour)+x, -60.0*cos(TAU*ColourFade.Colour)+y,{255,255,255});
			//DrawLine(x,y,60.0*sin(TAU*ColourFade.Target)+x, -60.0*cos(TAU*ColourFade.Target)+y,{255,255,255});
			//DrawText(x-60, y+60+10, "Colour:" + std::to_string(ColourFade.Colour) + "   Target:" + std::to_string(ColourFade.Target), {255,255,255});
		}
		//Graph(200, 600, 0.0, 1.0, 100.0, 100.0, RedVal, {255,0,0});
		//Graph(200, 600, 0.0, 1.0, 100.0, 100.0, GreenVal, {0,255,0});
		//Graph(200, 600, 0.0, 1.0, 100.0, 100.0, BlueVal, {0,0,255});
		
		Dance.Draw(10,30);
		
		u64 delta = GetMicroseconds() - lastTime;
		lastTime = GetMicroseconds();
		maxTime = MAX(delta, maxTime);
		DrawText(10, 10, "Runtime:" + std::to_string((lastTime-StartTime)/1'000'000.0) + "  Frametime:" + std::to_string(delta/1'000'000.0) + "  Maxtime:" + std::to_string(maxTime/1'000'000.0), {255,255,255});
		if ((lastTime-StartTime) % 1'00'000 < 1000) { maxTime = 0; }
		
		
	
		Sync();
		
		//u64 nextTime = GetMilliseconds();
		//usleep((100-(GetMilliseconds()-lastTime))*1000);
		//usleep(500);
	}
	
	
	DestroyWindow();
	return 0;
}
