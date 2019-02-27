
#include "Globals.h"
#include "WindowController.h"
#include "DanceController.h"
#include "LightStrip.h"

u64 StartTime = GetMilliseconds();
//TerminalBuffer TermBuffer(10, 900, 30);

DanceController Dance;
	
int main()
{
	InitWindow();
	srand(StartTime);
	
	//u64 startTime = GetMilliseconds();
	u64 lastTime = StartTime;
	
	bool run = true;
	while (run)
	{
		ForceClear();
		
		for (int i = 0; i < 100; i++)
		{
			DrawRectangle(0+10*i, 0, 20, 20, RGBVal({i/100.0,1.0}));
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
				if (key == 'w') { Dance.HalfTime = true; }
				if (key == 'e') { Dance.DoubleTime = true; }
			}
			else if (event.type == KeyRelease) 
			{ 
				key = GetKeyReleased(&event); 
				
				if (key == 'u') { Dance.StateIn[0] = 0; }
				if (key == 'i') { Dance.StateIn[1] = 0; }
				if (key == 'o') { Dance.StateIn[2] = 0; }
				if (key == 'p') { Dance.StateIn[3] = 0; }
				if (key == ' ') { Dance.BeatIn = 0; }
				
				if (key == 'w') { Dance.HalfTime = false; }
				if (key == 'e') { Dance.DoubleTime = false; }
			}
			
			if (key == 65307) { run = false; }
		}
		
		Dance.Update();
		Dance.Draw(10,30);
		DrawText(10, 10, "Runtime:" + std::to_string((lastTime-StartTime)/1000.0) + "  Frametime:" + std::to_string((GetMilliseconds() - lastTime)/1000.0), {255,255,255});
		lastTime = GetMilliseconds();
		
		
	
		Sync();
		
		//u64 nextTime = GetMilliseconds();
		//usleep((100-(GetMilliseconds()-lastTime))*1000);
		usleep(500);
	}
	
	
	DestroyWindow();
	return 0;
}
