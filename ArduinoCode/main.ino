#include <FastLED.h>

#define NUM_LEDS 300

#define DATA_PIN 9

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
		leds[i] = CRGB{255,255,255};

		FastLED.show();

		delay(10);
		
		leds[i] = CRGB::Black;
	}
	for(int i = 0; i < NUM_LEDS; i++) 
	{
		leds[i] = CRGB::Black;
	}
	FastLED.show();
	delay(10);
}