
#include <wiringPi.h>
#include "DanceController.h"
#include "LightStrip.h"
#include "LEDController.h"
//#include <thread>

u32 RGBVal(double val)
{		
	return (u8)(255.0*RedVal(val))<<16 | (u8)(255.0*GreenVal(val))<<8 | (u8)(255.0*BlueVal(val));
}


bool Run = true;

static void CloseHandler(int signum)
{
	Run = false;
	std::cout<<"\n";
}

void SetupHandlers()
{
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = CloseHandler;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}

DanceController Dance;
LEDController Controller;

void RenderThread()
{
	while (Run)
	{
		if (!Controller.Render()) { Run=false; }
	}
}



i64 Time1, Time2;

int main()
{
	std::thread renderThread;
	SetupHandlers();
	srand(StartTime);


	printf("Init\n");
	
	wiringPiSetup();
	wiringPiSetupGpio();
	//pinMode(16, OUTPUT);
	//digitalWrite(16, level); level=!level;

	if(!Controller.Init(300,0)) { return 1; }

	renderThread = std::thread(RenderThread);

	printf("Main Loop\n");
	i64 now = StartTime;
	i64 maxTime = 0;
	while (Run)
	{
		//std::cout << "Start Loop\n";
		//std::cout << "Render Done\n";

		Dance.Update();
		
		//std::cout << "Update Done\n";
		for (int i = 0; i < (int)Dance.LightStripList.size(); i++)
		{
			Controller.Draw(&Dance.LightStripList[i]);
		}
		//std::cout << "Draw Done\n";

		//Time1 = GetMicroseconds();
		//if (!Controller.Render()) { Run=false; }
		//Time2 = GetMicroseconds();

		i64 delta = GetMicroseconds() - now;
		now = GetMicroseconds();
		maxTime = MAX(delta, maxTime);
		static bool latch = false;
		if ((now-StartTime) % 100'000 < 50'000) { if (!latch) { std::cout << "\r" << "Runtime:" << std::to_string((now-StartTime)/1'000'000.0) << "  Frametime:" << std::to_string(delta/1'000'000.0) << "  Maxtime:" << std::to_string(maxTime/1'000'000.0) << "  Measure:" << std::to_string((Time2-Time1)/1'000'000.0) << std::flush; maxTime = 0; } latch = true; } else { latch = false; }

	}

	//renderThread.join();

	Controller.Destroy();
	
	printf("\nDone\n");
	return 0;
}
