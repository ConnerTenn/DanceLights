
#include "Globals.h"
#include "WindowController.h"
#include "DanceController.h"

//u64 StartTime = GetMilliseconds();
//TerminalBuffer TermBuffer(10, 900, 30);

DanceController Dance;
	
int main()
{
	InitWindow();
	
	u64 startTime = GetMilliseconds();
	u64 lastTime = startTime;
	
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
				
				if (key == 'u') { Dance.State[0] = 1; }
				if (key == 'i') { Dance.State[1] = 1; }
				if (key == 'o') { Dance.State[2] = 1; }
				if (key == 'p') { Dance.State[3] = 1; }
				if (key == ' ') { Dance.Beat = 1; }
			}
			else if (event.type == KeyRelease) 
			{ 
				key = GetKeyReleased(&event); 
				
				if (key == 'u') { Dance.State[0] = 0; }
				if (key == 'i') { Dance.State[1] = 0; }
				if (key == 'o') { Dance.State[2] = 0; }
				if (key == 'p') { Dance.State[3] = 0; }
				if (key == ' ') { Dance.Beat = 0; }
			}
			
			if (key == 65307) { run = false; }
		}
		
		Dance.Update();
		Dance.Draw(10,30);
		DrawText(10, 10, std::to_string((lastTime-startTime)/1000.0), {255,255,255});
		
		Sync();
		
		//u64 nextTime = GetMilliseconds();
		//usleep((100-(GetMilliseconds()-lastTime))*1000);
		usleep(1000);
		lastTime = GetMilliseconds();
	}
	
	
	DestroyWindow();
	return 0;
}
