
#include "LEDController.h"

bool LEDController::Init(int len1, int len2)
{
    memset(&LedDef, 0, sizeof(LedDef));
    LedDef.freq = WS2811_TARGET_FREQ;
	LedDef.dmanum = 10;

    ws2811_channel_t channel1, channel2;
	memset(&channel1, 0, sizeof(channel1));
	memset(&channel2, 0, sizeof(channel2));
	channel1.gpionum = 18;
	channel1.count = len1;
	channel1.invert = 0;
	channel1.brightness = 255;
	channel1.strip_type = WS2811_STRIP_GRB;

	channel2.gpionum = 13;
	channel2.count = len2;
	channel2.invert = 0;
	channel2.brightness = 255;
	channel2.strip_type = WS2811_STRIP_GRB;

	LedDef.channel[0] = channel1;
	LedDef.channel[1] = channel2;

    if((Ret = ws2811_init(&LedDef)) != WS2811_SUCCESS)
	{
		std::cerr << "ws2811_init failed: " << ws2811_get_return_t_str(Ret) << "\n";
        return false;
	}

    Clear();
    Render();

    return true;
}

void LEDController::Destroy()
{
    Clear();
    Render();
    ws2811_fini(&LedDef);
}

void LEDController::Clear()
{
    for (int i = 0; i < LedDef.channel[0].count; i++)
    {
        LedDef.channel[0].leds[i] = 0x000000;
    }
    for (int i = 0; i < LedDef.channel[1].count; i++)
    {
        LedDef.channel[1].leds[i] = 0x000000;
    }
}

bool LEDController::Render()
{
    if ((Ret = ws2811_render(&LedDef)) != WS2811_SUCCESS)
    {
        std::cerr << "ws2811_render failed: " << ws2811_get_return_t_str(Ret) << "\n";
        return false;
    }

    return true;
}

