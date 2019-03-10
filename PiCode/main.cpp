
#include "LEDController.h"

u32 RGBVal(double val)
{		
	return (u8)(255.0*RedVal(val))<<16 | (u8)(255.0*GreenVal(val))<<8 | (u8)(255.0*BlueVal(val));
}


bool Run = true;

static void ctrl_c_handler(int signum)
{
	(void)(signum);
	Run = false;
	std::cout<<"\n";
}

void setup_handlers()
{
	struct sigaction sa;// = { .sa_handler = ctrl_c_handler };
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = ctrl_c_handler;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}

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
	
	printf("Setup Handlers\n");

	setup_handlers();

	printf("Init\n");
	
	if(!Controller.Init(300,0))
	{
		return 1;
	}

	printf("Main Loop\n");
	
	while (Run)
	{
		Update();
		if (!Controller.Render()) { Run=false; }
		
		// 15 frames /sec
		usleep(1000000 / 15);
	}
	
	printf("Exiting...\n");

	Controller.Destroy();
	
	printf("Done\n");

	return 0;
}
