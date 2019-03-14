
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


enum ButtonNames
{
	RightIndex = 0,
	RightMiddle,
	RightRing,
	RightPinky,
	RightThumb,

	RightIndex2,
	RightMiddle2,
	RightRing2,
	RightPinky2,
	RightThumb2,
	
	LeftIndex,
	LeftMiddle,
	LeftRing,
	LeftPinky,
	LeftThumb,

	LeftIndex2,
	LeftMiddle2,
	LeftRing2,
	LeftPinky2,
	LeftThumb2,
};

u8 ButtonSetEnable[2] = {0,0};

u8 ButtonArray[][3]=
{
	{0, 2},
	{0, 3},
	{0, 4},
	{0, 17},
	{0, 27},

	{0, 2},
	{0, 3},
	{0, 4},
	{0, 17},
	{0, 27},

	{0, 22},
	{0, 9},
	{0, 11},
	{0, 5},
	{0, 6},

	{0, 22},
	{0, 9},
	{0, 11},
	{0, 5},
	{0, 6},
};

void UpdateButtons()
{
	for (int i = 0; i < 20; i++)
	{
		if (i%10==0) { digitalWrite(ButtonSetEnable[1-i/10], LOW); digitalWrite(ButtonSetEnable[i/10], HIGH); }

		ButtonArray[i][0] = digitalRead(ButtonArray[i][1]);
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
	pinMode(ButtonSetEnable[0], OUTPUT); pinMode(ButtonSetEnable[1], OUTPUT);
	for (int i = 0; i < 20; i++)
	{
		pinMode(ButtonArray[i][1], INPUT);
	}

	if(!Controller.Init(300,300,300)) { return 1; }

	renderThread = std::thread(RenderThread);

	printf("Main Loop\n");
	i64 now = StartTime;
	i64 maxTime = 0;
	while (Run)
	{
		UpdateButtons();
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

		//if (digitalRead(Extra1)) { Dance.Hold = true; } else { Dance.Hold = false; }
		//if (digitalRead(Extra2)) { Dance.Manual = true; } else { Dance.Manual = false; }
		//if (digitalRead(Extra3)) { Dance.ForceUpdate = true; } else { Dance.ForceUpdate = false; }

		//if (digitalRead(Extra4)) { Dance.MajorWeight = -1; }
		//if (digitalRead(Extra5)) { Dance.MajorWeight = 0; }
		//if (digitalRead(Extra6)) { Dance.MajorWeight = 1; }

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
