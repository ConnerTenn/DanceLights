
#include <wiringPi.h>
#include "DanceController.h"
//#include "LightStrip.h"
#include "LEDController.h"
#include <thread>

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


enum ButtonMaps
{
	RightIndex = 2,
	RightMiddle = 3,
	RightRing = 4,
	RightPinky = 17,
	RightThumb = 23,
	LeftIndex = 27,
	LeftMiddle = 22,
	LeftRing = 5,
	LeftPinky = 6,
	LeftThumb = 24
};

i64 Time1, Time2;

int main()
{
	std::thread renderThread;
	SetupHandlers();
	srand(StartTime);


	printf("Init\n");
	
	wiringPiSetup();
	wiringPiSetupGpio();
	pinMode(RightIndex, INPUT);
	pinMode(RightMiddle, INPUT);
	pinMode(RightRing, INPUT);
	pinMode(RightPinky, INPUT);
	pinMode(RightThumb, INPUT);
	pinMode(LeftIndex, INPUT);
	pinMode(LeftMiddle, INPUT);
	pinMode(LeftRing, INPUT);
	pinMode(LeftPinky, INPUT);
	pinMode(LeftThumb, INPUT);

	if(!Controller.Init(300,300,300)) { return 1; }

	renderThread = std::thread(RenderThread);

	printf("Main Loop\n");
	i64 now = StartTime;
	i64 maxTime = 0;
	while (Run)
	{
		if (digitalRead(RightIndex))  { Dance.NextStyle = Style::Pulse; }
		if (digitalRead(RightMiddle)) { Dance.NextStyle = Style::Streak; }
		if (digitalRead(RightRing))   { Dance.NextStyle = Style::StreakFade; }
		if (digitalRead(RightPinky))  { Dance.NextStyle = Style::FlipFlop; }
		if (digitalRead(RightThumb))  { Dance.NextStyle = Style::Fade; }

		if (digitalRead(LeftIndex))  { Dance.Speed = -2; }
		if (digitalRead(LeftMiddle)) { Dance.Speed = -1; }
		if (digitalRead(LeftRing))   { Dance.Speed = 1; }
		if (digitalRead(LeftPinky))  { Dance.Speed = 2; }
		if (digitalRead(LeftThumb))  { Dance.Speed = 0; }

		Dance.Update();
		
		for (int i = 0; i < (int)Dance.LightStripList.size(); i++)
		{
			Controller.Draw(Dance.LightStripList[i]);
		}
		
		//if (!Controller.Render()) { Run=false; }

		i64 delta = GetMicroseconds() - now;
		now = GetMicroseconds();
		maxTime = MAX(delta, maxTime);
		static bool latch = false;
		if ((now-StartTime) % 100'000 < 50'000) { if (!latch) { std::cout << "\r" << "Runtime:" << std::to_string((now-StartTime)/1'000'000.0) << "  Frametime:" << std::to_string(delta/1'000'000.0) << "  Maxtime:" << std::to_string(maxTime/1'000'000.0) << "  Measure:" << std::to_string((Time2-Time1)/1'000'000.0) << std::flush; maxTime = 0; } latch = true; } else { latch = false; }

	}

	renderThread.join();

	Controller.Destroy();
	
	printf("\nDone\n");
	return 0;
}
