
#include <wiringPi.h>
#include "DanceController.h"
#include "LightStrip.h"
#include "LEDController.h"

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

void Update()
{
	for (int i = 0; i < Controller.LedDef.channel[0].count; i++)
	{
		static double t = 0; t+= 0.0001;
		Controller.LedDef.channel[0].leds[i] = RGBVal(fmod(i/(300-1.0) + t,1.0));
	}
}

int main()
{
	SetupHandlers();
	srand(StartTime);


	printf("Init\n");
	
	wiringPiSetup();
	wiringPiSetupGpio();
	//pinMode(16, OUTPUT);
	//digitalWrite(16, level); level=!level;

	if(!Controller.Init(300,0)) { return 1; }

	printf("Main Loop\n");
	i64 now = StartTime;
	i64 maxTime = 0;
	while (Run)
	{
		Update();
		if (!Controller.Render()) { Run=false; }

		Dance.Update();		
		// 15 frames /sec
		//usleep(1000000 / 15);

		i64 delta = GetMicroseconds() - now;
		now = GetMicroseconds();
		maxTime = MAX(delta, maxTime);
		static bool latch = false;
		if ((now-StartTime) % 500'000 < 100'000) { if (!latch) { std::cout << "\r" << "Runtime:" << (now-StartTime)/1'000'000.0 << "  Frametime:" << delta/1'000'000.0 << "  Maxtime:" << maxTime/1'000'000.0 << std::flush; maxTime = 0; } latch = true; } else { latch = false; }

	}

	Controller.Destroy();
	
	printf("Done\n");
	return 0;
}
