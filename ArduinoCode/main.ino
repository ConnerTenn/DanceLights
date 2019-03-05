#include <FastLED.h>

#define NUM_LEDS 300

#define DATA_PIN 9

#define DIVISOR 2

#define MIN(a,b) ((a)<=(b)?(a):(b))
#define MAX(a,b) ((a)>=(b)?(a):(b))
#define ABS(a) ((a)<0?-(a):(a))
#define MOD(a,b) ((a)%(b)+((a)<0?(b):0))

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
inline CRGB RGBVal(double val)
{		
	return CRGB{(u8)(255.0*RedVal(val)),(u8)(255.0*GreenVal(val)),(u8)(255.0*BlueVal(val))};
}
inline CRGB RGBScale(CRGB rgb, double scale)
{
	return CRGB{ (u8)(scale*rgb.r), (u8)(scale*rgb.g), (u8)(scale*rgb.b) };
}

CRGB leds[NUM_LEDS];

void setup()
{
	delay(2000);
	FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
}

void loop() 
{
	for(int i = 0; i < NUM_LEDS; i++) 
	{
		leds[i] = RGBScale(RGBVal(fmod((double)millis()/3000.0+(NUM_LEDS-i)/300.0,1.0)), 0.6);//CRGB{0/DIVISOR,125/DIVISOR,255/DIVISOR};
	}
	FastLED.show();
	
	/*for(int i = 0; i < NUM_LEDS; i++) 
	{
		leds[i] = CRGB{255,255,255};

		FastLED.show();

		delay(10);
		
		//leds[i] = CRGB::Black;
	}
	delay(500-10);
	for(int i = 0; i < NUM_LEDS; i++) 
	{
		leds[i] = CRGB{0,0,0};

		FastLED.show();

		delay(10);
		
		//leds[i] = CRGB::Black;
	}*/
}