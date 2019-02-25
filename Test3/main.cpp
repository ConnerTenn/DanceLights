
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
		
		for (int i = 0; i < 200; i++)
		{
			DrawRectangle(0+10*i, 0, 10, 20, ColourVal(i/200.0));
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
		
		
		
		Sync();
		
		//u64 nextTime = GetMilliseconds();
		//usleep((100-(GetMilliseconds()-lastTime))*1000);
		usleep(1000);
	}
	
	
	DestroyWindow();
	return 0;
}
