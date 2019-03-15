
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
	//RightThumb,
	RightIndex2,
	RightMiddle2,
	RightRing2,
	RightPinky2,
	//RightThumb2,
	
	LeftIndex,
	LeftMiddle,
	LeftRing,
	LeftPinky,
	//LeftThumb,
	LeftIndex2,
	LeftMiddle2,
	LeftRing2,
	LeftPinky2,
	//LeftThumb2,
};

//Right, Left
u8 ButtonSetEnable[2] = {6,5};

u8 ButtonArray[][3]=
{
	//Right
	{0, 17},
	{0, 7},
	{0, 27},
	{0, 11},
	{0, 10},
	{0, 8},
	{0, 22},
	{0, 25},

	//Left
	{0, 11},
	{0, 27},
	{0, 7},
	{0, 17},
	{0, 25},
	{0, 22},
	{0, 8},
	{0, 10},
};

void UpdateButtons()
{
	digitalWrite(ButtonSetEnable[0], HIGH);
	usleep(10);
	for (int i = 0; i < 8; i++)
	{
		ButtonArray[i][0] = digitalRead(ButtonArray[i][1]);
	}
	digitalWrite(ButtonSetEnable[0], LOW);
	digitalWrite(ButtonSetEnable[1], HIGH);
	usleep(10);
	for (int i = 8; i < 16; i++)
	{
		ButtonArray[i][0] = digitalRead(ButtonArray[i][1]);
	}
	digitalWrite(ButtonSetEnable[0], LOW);
	digitalWrite(ButtonSetEnable[1], LOW);
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
	for (int i = 0; i < 8; i++)
	{
		pinMode(ButtonArray[i][1], INPUT);
		pullUpDnControl(ButtonArray[i][1], PUD_DOWN);
	}
	digitalWrite(ButtonSetEnable[0], HIGH); digitalWrite(ButtonSetEnable[1], HIGH);
	
	if(!Controller.Init(300,300,300)) { return 1; }

	renderThread = std::thread(RenderThread);

	printf("Main Loop\n");
	i64 now = StartTime;
	i64 maxTime = 0;
	while (Run)
	{
		UpdateButtons();

		//std::cout << "\r";
		for (int i = 0; i < 16; i++)
		{
			//std::cout << (bool)ButtonArray[i][0] << " ";
			//if (i==7) { std::cout << " "; }
		}
		//std::cout << std::flush ;
		Dance.BeatIn = ButtonArray[RightIndex][0];
		if (ButtonArray[RightPinky2][0]) { Dance.Beat.Period = 0; Dance.Beat.Align = 0; }

		if (ButtonArray[LeftIndex][0])  { Dance.NextStyle = Style::Pulse; }
		if (ButtonArray[LeftMiddle][0]) { Dance.NextStyle = Style::Streak; }
		if (ButtonArray[LeftRing][0])   { Dance.NextStyle = Style::StreakFade; }
		if (ButtonArray[LeftPinky][0])  { Dance.NextStyle = Style::Fade; }
		//if (ButtonArray[Middle][0])  { Dance.NextStyle = Style::FlipFlop; }

		if (ButtonArray[LeftIndex2][0])  { Dance.Speed = 0; }
		if (ButtonArray[LeftMiddle][0])  { Dance.Speed = 1; }
		if (ButtonArray[LeftRing2][0])   { Dance.Speed = -1; }
		if (ButtonArray[LeftPinky2][0])  { Dance.Speed = -2; }

		if (ButtonArray[RightRing2][0]) { Dance.Hold = true; } else { Dance.Hold = false; }
		if (ButtonArray[RightMiddle2][0])   { Dance.Manual = true; } else { Dance.Manual = false; }
		if (ButtonArray[RightIndex2][0])  { Dance.ForceUpdate = true; } else { Dance.ForceUpdate = false; }

		if (ButtonArray[RightMiddle][0]) { Dance.MajorWeight = -1; }
		if (ButtonArray[RightRing][0])   { Dance.MajorWeight = 0; }
		if (ButtonArray[RightPinky][0])  { Dance.MajorWeight = 1; }
		
		Dance.BeatIn = digitalRead(26);
		//std::cout << (int)Dance.BeatIn << "\n";

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
