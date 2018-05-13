//
//
//

#include "LED_Framework.h"

CRGB STRIP_LEDs[NUM_LEDS];
CRGB Foreground;
#ifdef POLE
LEDStrip Strips[4]{
	{ 3,57 },
{ 116,62 },
{ 121, 175 },
{ 232, 178 }
};
#else
LEDStrip Strips[2]{
	{ 29,0 },
{30, 59}
};
#endif
bool bypassBrightness = false;
bool stateOn = true;
uint8_t brightness;
StopWatch FrameTimer;
uint8_t FrameTime;
uint16_t global_param;
uint8_t numStrips = sizeof(Strips) / sizeof(Strips[0]);
String effectString;

void(*Effect)();

void setEffect(void(*NewEffect)(), String EffString)
{
	Effect = NewEffect;
	effectString = EffString;
}

String getEffect()
{
	return effectString;
}

void LEDSetup()
{
	FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_CURRENT);
	FastLED.setCorrection(TypicalPixelString);
	FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(STRIP_LEDs, NUM_LEDS);
	setFrameRate(30);
	FastLED.setMaxRefreshRate(120, true);
	Foreground = CRGB(32, 0, 0);
	Effect = SOLID;
}

void LEDLoop()
{
	if (FrameTimer.getTime() > FrameTime)
	{
		FrameTimer.reset();
		Effect();
	}
	FastLED.show();
}

bool Roll(unsigned int chance)
{
	return (random16(10000) < chance);
}

uint8_t stripWraparound(int idx)
{
	while (idx >= numStrips) idx -= numStrips;
	while (idx < 0) idx += numStrips;
}

void setBrightness(uint8_t newBright)
{
	brightness = newBright;
	if (bypassBrightness)
	{
		return;
	}
	FastLED.setBrightness(brightness);
}

void setFrameRate(float FPS)
{
	FrameTime = 1000.0 / FPS;
}

LEDStrip::LEDStrip(uint8_t start, uint8_t stop)
{
	if (start > (NUM_LEDS - 1)) start = NUM_LEDS - 1;
	if (stop > (NUM_LEDS - 1)) stop = NUM_LEDS - 1;
	if (stop > start) //We are going forward
	{
		size = stop - start + 1;
		LEDs = new LED[size];
		//Allocate LEDs in forward order
		for (int i = 0; i < size; i++)
		{
			LEDs[i].address = start + i;
		}
	}
	else //We are going in reverse
	{
		size = start - stop + 1;
		LEDs = new LED[size];
		//Allocate LEDs in reverse order
		for (int i = 0; i < size; i++)
		{
			LEDs[i].address = start - i;
		}
	}
}

LEDStrip::~LEDStrip()
{
	delete[] LEDs;
}

bool LEDStrip::setColor(uint8_t idx, CRGB color)
{
	if (idx >= size) return false;
	STRIP_LEDs[LEDs[idx].address] = color;
}

CRGB LEDStrip::getColor(int idx)
{
	if (idx < size && idx >= 0)
	{
		return STRIP_LEDs[LEDs[idx].address];
	}
	else
	{
		return CRGB::Black;
	}
}

uint16_t LEDStrip::getParam(int idx)
{
	if (idx < size && idx >= 0)
	{
		return LEDs[idx].param;
	}
	else
	{
		return 0;
	}
}

void SOLID()
{
	for (uint8_t s = 0; s < numStrips; s++)
	{
		for (uint8_t p = 0; p < Strips[s].size; p++)
		{
			Strips[s].setColor(p, Foreground);
		}
	}
}