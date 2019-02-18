
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
	
	std::string lastKey = "---";
	
	bool run = true;
	while (run)
	{
		ForceClear();
		
		u64 key;
		XEvent event;
		if (PendingEvent(&event))
		{
			if (event.type == KeyPress) { key = GetKeyPressed(&event); }
			else if (event.type == KeyRelease) { key = GetKeyReleased(&event); }
			
			lastKey = std::to_string(key) + " " + (char)(key);
			
			if (key == 'u') { Dance.State[0] = 1; }
			
			if (key == 65307) { run = false; }
		}
		
		DrawText(10, 10, std::to_string((lastTime-startTime)/1000.0), {255,255,255});
		DrawText(10, 30, lastKey, {255,255,255});
		
		Sync();
		
		//u64 nextTime = GetMilliseconds();
		//usleep((100-(GetMilliseconds()-lastTime))*1000);
		usleep(1000);
		lastTime = GetMilliseconds();
	}
	
	
	DestroyWindow();
	return 0;
}
