
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
	
	LightStrip strip[5] = {LightStrip(50),LightStrip(50),LightStrip(50),LightStrip(50),LightStrip(50)};
	
	bool run = true;
	while (run)
	{
		ForceClear();
		
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
			}
			else if (event.type == KeyRelease) 
			{ 
				key = GetKeyReleased(&event); 
				
				if (key == 'u') { Dance.StateIn[0] = 0; }
				if (key == 'i') { Dance.StateIn[1] = 0; }
				if (key == 'o') { Dance.StateIn[2] = 0; }
				if (key == 'p') { Dance.StateIn[3] = 0; }
				if (key == ' ') { Dance.BeatIn = 0; }
			}
			
			if (key == 65307) { run = false; }
		}
		
		Dance.Update();
		Dance.Draw(10,30);
		DrawText(10, 10, "Runtime:" + std::to_string((lastTime-StartTime)/1000.0) + "  Frametime:" + std::to_string((GetMilliseconds() - lastTime)/1000.0), {255,255,255});
		lastTime = GetMilliseconds();
		
		
		/*static bool latch = false;
		if (Dance.Beat(lastTime, Dance.Beat.Frequency/2)) // Dance.UpdateCycle.Frequency))
		{
			static double val = 0;
			for (int i = 0; i < strip.Length && !latch; i++)
			{
				//RGB rgb{(u8)rand(), (u8)rand(), (u8)rand()};
				strip.Lights[i] = ColourVal(fmod(val+i/50.0,1.0));
				
				//val += 0.05/strip.Lights.Size();
			}
			//std::cout << "ADDR:"<<strip.Lights.Values<<"\n";
			val += 0.001;
			latch = true;
		}
		else { latch = false; }*/
		Style style;
		style.Speed = Dance.Beat.Frequency; style.Align = Dance.Beat.Align;
		strip[0].Update(&style);
		style.Speed = Dance.Beat.Frequency; style.Align = Dance.Beat.Align + Dance.Beat.Frequency/30;
		strip[1].Update(&style);
		style.Speed = Dance.Beat.Frequency; style.Align = Dance.Beat.Align + 2*Dance.Beat.Frequency/30;
		strip[2].Update(&style);
		style.Speed = Dance.Beat.Frequency; style.Align = Dance.Beat.Align + 3*Dance.Beat.Frequency/30;
		strip[3].Update(&style);
		style.Speed = Dance.Beat.Frequency; style.Align = Dance.Beat.Align + 4*Dance.Beat.Frequency/30;
		strip[4].Update(&style);
		
		for (int i = 0; i < 5; i++)
		{
			strip[i].Draw(150, 150+i*30, 0);
		}
		
		
		Sync();
		
		//u64 nextTime = GetMilliseconds();
		//usleep((100-(GetMilliseconds()-lastTime))*1000);
		usleep(1000);
	}
	
	
	DestroyWindow();
	return 0;
}
