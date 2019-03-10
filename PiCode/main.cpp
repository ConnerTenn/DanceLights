
#include <iostream>
#include <ws2811.h>

inline double RedVal(double val)
{
	return val = 2.0-abs(6.0*(0.5-abs(val-0.5))), val=MIN(val,1.0), MAX(val,0.0);
}
inline double GreenVal(double val)
{
	return val = 2.0-abs(6.0*val-2.0), val=MIN(val,1.0), MAX(val,0.0);
}
inline double BlueVal(double val)
{
	return val = 2.0-abs(6.0*val-4.0), val=MIN(val,1.0), MAX(val,0.0);
}
inline unsigned int RGBVal(ColourVal val)
{		
	return (u8)(255.0*RedVal(val.Colour)*val.Scale)<<16 | (u8)(255.0*GreenVal(val.Colour)*val.Scale)<<8 | (u8)(255.0*BlueVal(val.Colour)*val.Scale);
}

bool Run = true;
ws2811_led_t Strip[300];
ws2811_t ledstring =
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
};

static void ctrl_c_handler(int signum)
{
	(void)(signum);
	Run = false;
}

void setup_handlers()
{
	sigaction sa = { .sa_handler = ctrl_c_handler };
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
}

void Clear()
{
	for (int i = 0; i < 300; i++)
	{
		Strip[i] = 0;
	}
}

void Update()
{
	for (int i = 0; i < 300; i++)
	{
		Strip[i] = RGBVal(((i+time()/100)%300)/(300.0-1.0));
	}
}

void Render()
{
	for (int i = 0; i < 300; i++)
	{
		ledstring.channel[0].leds[i] = matrix[i];
	}
}

int main()
{
	ws2811_return_t ret;
	
	
	
	setup_handlers();
	
	if((ret = ws2811_init(&ledstring)) != WS2811_SUCCESS)
	{
		std::cerr << "ws2811_init failed: " << ws2811_get_return_t_str(ret) << "\n";
		return ret;
	}
	
	while (Run)
	{
		Update();
		Render();

		if ((ret = ws2811_render(&ledstring)) != WS2811_SUCCESS)
		{
			std::cerr << "ws2811_render failed: " << ws2811_get_return_t_str(ret) << "\n";
			break;
		}

		// 15 frames /sec
		usleep(1000000 / 15);
	}
	
	std::cout << "Exiting...\n";


	Clear();
	Render();
	ws2811_render(&ledstring);

    ws2811_fini(&ledstring);
	
	std::cout << "Done\n";

	return 0;
}
