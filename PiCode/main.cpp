
#include <math.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>
#include <stdarg.h>
#include <getopt.h>

#include <clk.h>
#include <gpio.h>
#include <dma.h>
#include <pwm.h>

#include <ws2811.h>

#define MIN(a,b) ((a)<=(b)?(a):(b))
#define MAX(a,b) ((a)>=(b)?(a):(b))

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

double RedVal(double val)
{
	return val = 2.0-abs(6.0*(0.5-abs(val-0.5))), val=MIN(val,1.0), MAX(val,0.0);
}
double GreenVal(double val)
{
	return val = 2.0-abs(6.0*val-2.0), val=MIN(val,1.0), MAX(val,0.0);
}
double BlueVal(double val)
{
	return val = 2.0-abs(6.0*val-4.0), val=MIN(val,1.0), MAX(val,0.0);
}
u32 RGBVal(double val)
{		
	return (u8)(255.0*RedVal(val))<<16 | (u8)(255.0*GreenVal(val))<<8 | (u8)(255.0*BlueVal(val));
}

#define TARGET_FREQ             WS2811_TARGET_FREQ
#define GPIO_PIN                18
#define DMA                     10
#define STRIP_TYPE              WS2811_STRIP_GRB		// WS2812/SK6812RGB integrated chip+leds
#define LED_COUNT 300


bool Run = true;
ws2811_led_t Strip[LED_COUNT];

ws2811_t ledstring;
/*ws2811_t ledstring =
{
    .freq = TARGET_FREQ,
    .dmanum = DMA,
    .channel =
    {
        [0] =
        {
            .gpionum = GPIO_PIN,
            .count = LED_COUNT,
            .invert = 0,
            .brightness = 255,
            .strip_type = STRIP_TYPE,
        },
        [1] =
        {
            .gpionum = 0,
            .count = 0,
            .invert = 0,
            .brightness = 0,
        },
    },
};*/

static void ctrl_c_handler(int signum)
{
	(void)(signum);
	Run = false;
}

void setup_handlers()
{
	struct sigaction sa;// = { .sa_handler = ctrl_c_handler };
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = ctrl_c_handler;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}

void Clear()
{
	for (int i = 0; i < LED_COUNT; i++)
	{
		Strip[i] = 0;
	}
}

void Update()
{
	for (int i = 0; i < LED_COUNT; i++)
	{
		static double t = 0; t+= 0.0001;
		Strip[i] = RGBVal(fmod(i/(LED_COUNT-1.0) + t,1.0));
	}
}

void Render()
{
	for (int i = 0; i < LED_COUNT; i++)
	{
		ledstring.channel[0].leds[i] = Strip[i];
	}
}

int main()
{
	ledstring.freq = TARGET_FREQ;
	ledstring.dmanum = DMA;
	
	ws2811_channel_t channel1, channel2;
	memset(&channel1, 0, sizeof(channel1));
	memset(&channel2, 0, sizeof(channel2));
	channel1.gpionum = GPIO_PIN;
	channel1.count = LED_COUNT;
	channel1.invert = 0;
	channel1.brightness = 255;
	channel1.strip_type = STRIP_TYPE;

	ledstring.channel[0] = channel1;
	ledstring.channel[1] = channel2;
	
	ws2811_return_t ret;
	
	printf("Setup Handlers\n");

	setup_handlers();

	printf("Init\n");
	
	if((ret = ws2811_init(&ledstring)) != WS2811_SUCCESS)
	{
		fprintf(stderr, "ws2811_init failed: %s\n", ws2811_get_return_t_str(ret));
		return ret;
	}

	printf("Main Loop\n");
	
	while (Run)
	{
		Update();
		Render();

		if ((ret = ws2811_render(&ledstring)) != WS2811_SUCCESS)
		{
			fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(ret));
			break;
		}
		
		// 15 frames /sec
		usleep(1000000 / 15);
	}
	
	printf("Exiting...\n");


	Clear();
	Render();
	ws2811_render(&ledstring);
	
	printf("Cleared...\n");

	ws2811_fini(&ledstring);
	
	printf("Done\n");

	return 0;
}
