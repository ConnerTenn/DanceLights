
#ifndef _LED_CONTROLLER_
#define _LED_CONTROLLER_

#include "Globals.h"
#include "LightStrip.h"

#include <stdint.h>
#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <fcntl.h>
//#include <sys/mman.h>
#include <signal.h>
//#include <stdarg.h>
//#include <getopt.h>

#include <clk.h>
#include <gpio.h>
#include <dma.h>
#include <pwm.h>

#include <ws2811.h>

/*inline u32 RGBToHEX(RGB val)
{
    return val.R<<16 | val.G<<8 | val.B;
}*/

#define RGBToHEX(val) (val.R<<16 | val.G<<8 | val.B)

class LEDController
{
public:
    ws2811_return_t Ret;
    ws2811_t LedDef;

    bool Init(int len1, int len2);
    void Destroy();

    void Clear();
    bool Render();

    void Draw(LightStrip *strip);
};

#endif 
