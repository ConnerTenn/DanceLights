
#include "LEDController.h"

bool LEDController::Init(int len1, int len2, int len3)
{
    {
        memset(&LedDef1, 0, sizeof(LedDef1));
        LedDef1.freq = WS2811_TARGET_FREQ;
        LedDef1.dmanum = 10;

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

        LedDef1.channel[0] = channel1;
        LedDef1.channel[1] = channel2;
    }
    {
        memset(&LedDef2, 0, sizeof(LedDef2));
        LedDef2.freq = WS2811_TARGET_FREQ;
        LedDef2.dmanum = 10;

        ws2811_channel_t channel1;
        memset(&channel1, 0, sizeof(channel1));
        channel1.gpionum = 21;
        channel1.count = len1;
        channel1.invert = 0;
        channel1.brightness = 255;
        channel1.strip_type = WS2811_STRIP_GRB;

        LedDef2.channel[0] = channel1;
    }

    if((Ret = ws2811_init(&LedDef1)) != WS2811_SUCCESS)
	{
		std::cerr << "ws2811_init 1 failed: " << ws2811_get_return_t_str(Ret) << "\n";
        return false;
	}
    if((Ret = ws2811_init(&LedDef2)) != WS2811_SUCCESS)
	{
		std::cerr << "ws2811_init 2 failed: " << ws2811_get_return_t_str(Ret) << "\n";
        return false;
	}
    std::cout << LedDef1.channel[0].count << "\n";
    std::cout << LedDef1.channel[1].count << "\n";
    std::cout << LedDef2.channel[0].count << "\n";

    Clear();

    return true;
}

void LEDController::Destroy()
{
    Clear();
    ws2811_fini(&LedDef1);
    ws2811_fini(&LedDef2);
}

void LEDController::Clear()
{
    for (int i = 0; i < LedDef1.channel[0].count; i++)
    {
        LedDef1.channel[0].leds[i] = 0x000000;
    }
    for (int i = 0; i < LedDef1.channel[1].count; i++)
    {
        LedDef1.channel[1].leds[i] = 0x000000;
    }
    for (int i = 0; i < LedDef2.channel[0].count; i++)
    {
        LedDef2.channel[0].leds[i] = 0x000000;
    }

    Render();
}

bool LEDController::Render()
{
    if ((Ret = ws2811_render(&LedDef1)) != WS2811_SUCCESS)
    {
        std::cerr << "ws2811_render 1 failed: " << ws2811_get_return_t_str(Ret) << "\n";
        return false;
    }
    if ((Ret = ws2811_render(&LedDef2)) != WS2811_SUCCESS)
    {
        std::cerr << "ws2811_render 2 failed: " << ws2811_get_return_t_str(Ret) << "\n";
        return false;
    }

    return true;
}

void LEDController::Draw(LightContainer *strip)
{
    ws2811_channel_t *channel = (strip->Channel <= 1 ? &LedDef1.channel[strip->Channel] : &LedDef2.channel[0]);
    int max = MIN(channel->count-strip->StripOffset, strip->Length);
    ws2811_led_t *leds = channel->leds;
    for (int i = 0; i < max; i++)
    {
        leds[i+strip->StripOffset] = RGBToHEX(strip->Lights[i]);
    }
}

