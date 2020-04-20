//
//
//

#include "LED_Framework.h"
#include <NeoPixelBus.h>
#include "Color_Correction.h"

CRGB STRIP_LEDs[NUM_LEDS];
CRGB Foreground;

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> FinalStrip(NUM_LEDS);

LEDStrip STRIP_CONFIG

bool bypassBrightness = false;
bool stateOn = true;
uint8_t brightness;
StopWatch FrameTimer;
uint8_t FrameTime;
StopWatch AnimPerfMon;
StopWatch DitherFrameTimer;
uint8_t DitherFrameTime;
uint16_t global_param;
uint8_t numStrips = sizeof(Strips) / sizeof(Strips[0]);
String effectString;

uint16_t dithercount = 0;
uint16_t framecount = 0;
uint16_t dithercyclecount = 0;


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
	FinalStrip.Begin();
	FinalStrip.ClearTo(RgbColor(0, 0, 0));
	FinalStrip.Show();
	//FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_CURRENT);
	//FastLED.setCorrection(TypicalPixelString);
	//FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(STRIP_LEDs, NUM_LEDS);
	//FastLED.setMaxRefreshRate(50, true);
	setFrameRate(4, 50);
	Foreground = CRGB(32, 0, 0);
	Effect = SOLID;
}



void LEDLoop()
{
	
	if (FrameTimer.repeat(FrameTime))
	{
		Effect();
		framecount++;
#ifdef DISABLE_DITHER
		if (FinalStrip.CanShow()) BasicShow();
#endif
	}

#ifndef DISABLE_DITHER
	if (FinalStrip.CanShow())
	{
		if (DitherFrameTimer.repeat(DitherFrameTime))
		{
			AdvancedShow();
			dithercount++;
		}

	}
#endif

	if (AnimPerfMon.repeat(10000))
	{
		DB.Message(DM_TIMING, "Anim FPS: " + String(framecount / 10) + " Dither FPS: " + String(dithercount / 10) + " Dither Cycle Rate: " + String(dithercyclecount / 10));
		framecount = 0;
		dithercount = 0;
		dithercyclecount = 0;
	}
	//FastLED.show();
}

void AdvancedShow()
{
	static uint32_t d_c;
	static uint32_t p_r;
	static uint32_t p_g;
	static uint32_t p_b;

	uint32_t num = (uint32_t)brightness * DITHER_COUNT;
	uint32_t den = CORRECTION_PRECISION * 255;
	
	for (uint8_t i; i < NUM_LEDS; i++)
	{
		//p_r = PRECISE_RED[STRIP_LEDs[i].r]/257;
		//p_g = PRECISE_RED[STRIP_LEDs[i].g] / 257;
		//p_b = PRECISE_RED[STRIP_LEDs[i].b] / 257;

		p_r = PRECISE_RED[STRIP_LEDs[i].r] * num / den;
		p_r = p_r + d_c;
		p_r /= DITHER_COUNT;
		
		
		p_g = PRECISE_GREEN[STRIP_LEDs[i].g] * num / den;
		p_g = p_g + d_c;
		p_g /= DITHER_COUNT;
		
		
		p_b = PRECISE_BLUE[STRIP_LEDs[i].b] * num / den;
		p_b = p_b + d_c;
		p_b /= DITHER_COUNT;

		if (p_r > 255) p_r = 255;
		if (p_g > 255) p_g = 255;
		if (p_b > 255) p_b = 255;

		FinalStrip.SetPixelColor(i, RgbColor(p_r, p_g, p_b));
	}

	FinalStrip.Show();

	//Cycle Dither Counter
	d_c++;
	if (d_c >= DITHER_COUNT)
	{
		d_c = 0;
		dithercyclecount++;
	}
}

void BasicShow()
{
	uint32_t num = brightness;
	uint32_t den = CORRECTION_PRECISION * 255;

	static uint32_t p_r;
	static uint32_t p_g;
	static uint32_t p_b;

	for (uint8_t i; i < NUM_LEDS; i++)
	{
		p_r = PRECISE_RED[STRIP_LEDs[i].r] * num / den;
		p_g = PRECISE_GREEN[STRIP_LEDs[i].g] * num / den;
		p_b = PRECISE_BLUE[STRIP_LEDs[i].b] * num / den;


		if (p_r > 255) p_r = 255;
		if (p_g > 255) p_g = 255;
		if (p_b > 255) p_b = 255;
		FinalStrip.SetPixelColor(i, RgbColor(p_r, p_g, p_b));
	}
	FinalStrip.Show();
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
}

void setFrameRate(float FPS)
{
	setFrameRate(FPS, 100);
}

void setFrameRate(float FPS, float DitherFPS)
{
	FrameTime = 1000.0 / FPS;
	DitherFrameTime = 1000.0 / DitherFPS;
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