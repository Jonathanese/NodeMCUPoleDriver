//
//
//

#include "LED_Framework.h"
#include <NeoPixelBus.h>
#include "Color_Correction.h"

CRGB STRIP_LEDs[NUM_LEDS];
CRGB Foreground;

#ifdef IS_RGBW
NeoPixelBus<NeoGrbwFeature, Neo800KbpsMethod> FinalStrip(NUM_LEDS);
#else
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> FinalStrip(NUM_LEDS);
#endif

StopWatch TestTimer;

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
	setFrameRate(NONE_FPS);
	Foreground = CRGB(32, 0, 0);
	Effect = SOLID;
}

void LEDLoop()
{
	if (FrameTimer.repeat(FrameTime))
	{
		Effect();
		framecount++;
#ifdef NO_DITHERING
		if (FinalStrip.CanShow()) BasicShow();
#endif
	}

#ifndef NO_DITHERING
	if (FinalStrip.CanShow())
	{
		AdvancedShow();
		dithercount++;
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
	static uint32_t d_c; //Counter for dithering. Used to alter how things are rounded when we divide.
	static uint32_t p_r; //Temporary values for processing channels
	static uint32_t p_g;
	static uint32_t p_b;
#ifdef IS_RGBW
	static uint32_t p_w; //White channel only necessary for RGBW LEDs
#endif

	//Numerator and Denominator are separated for integer division. Put as many constants up here as possible to save on calculations during the FOR loop.
	uint32_t num = (uint32_t)brightness * DITHER_COUNT;
	uint32_t den = CORRECTION_PRECISION * 255;

	for (uint8_t i = 0; i < NUM_LEDS; i++)
	{

		//Perform gamma correction to convert the desired perceived brightness into actual brightness values

		p_r = (uint32_t)GAMMA_LUT[STRIP_LEDs[i].r];
		p_g = (uint32_t)GAMMA_LUT[STRIP_LEDs[i].g];
		p_b = (uint32_t)GAMMA_LUT[STRIP_LEDs[i].b];

		//We are now in "Linear Space". Averaging, subtracting, and scaling values will all produce correct results.

		
#ifdef IS_RGBW
		//Subtract white values while in linear space
		p_w = min(p_r, min(p_g, p_b)); //The minimum of the 3 channels is the amount of "whiteness" that can be removed
		p_r = p_r - p_w; //Remove the redundant "whiteness" from the RGB values
		p_g = p_g - p_w;
		p_b = p_b - p_w;

		//These are the same steps as below. I am just performing them on White all at once right here.
		p_w = p_w * CORRECTION_W / 255;
		p_w = p_w * num / den;
		p_w = p_w + d_c;
		p_w /= DITHER_COUNT;
		if (p_w > 255) p_w = 255;
#endif


		//Scale values according to color correction / white balance
		p_r = p_r * CORRECTION_R / 255;
		p_g = p_g * CORRECTION_G / 255;
		p_b = p_b * CORRECTION_B / 255;


		// Take out most of the constants. This will bring us to our final values X the number of dithering steps.
		p_r = p_r * num / den;
		p_g = p_g * num / den;
		p_b = p_b * num / den;


		//Increment the dithering steps. That way when we finally divide out DITHER_COUNT, we will get rounding results that change depending on where the actual value lies between the discrete values.
		p_r = p_r + d_c;
		p_g = p_g + d_c;
		p_b = p_b + d_c;

		//Divide by the dither count to get the final 0-255 values.
		p_r /= DITHER_COUNT;
		p_g /= DITHER_COUNT;
		p_b /= DITHER_COUNT;

		//Sometimes the integer math may produce results like 256. Clamp these to 255 to avoid wraparound.
		if (p_b > 255) p_b = 255;
		if (p_g > 255) p_g = 255;
		if (p_r > 255) p_r = 255;

#ifdef IS_RGBW
		FinalStrip.SetPixelColor(i, RgbwColor(p_r, p_g, p_b, p_w));
#else
		FinalStrip.SetPixelColor(i, RgbColor(p_r, p_g, p_b));
#endif
		
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
	static uint32_t num = brightness;
	static uint32_t den = CORRECTION_PRECISION * 255;

	static uint32_t p_r;
	static uint32_t p_g;
	static uint32_t p_b;

#ifdef IS_RGBW
	static uint32_t p_w;
#endif


	for (uint8_t i; i < NUM_LEDS; i++)
	{

		//Use LUT to convert exponential space to final, linear space
		p_r = (uint32_t)GAMMA_LUT[STRIP_LEDs[i].r] * num / den;
		p_g = (uint32_t)GAMMA_LUT[STRIP_LEDs[i].g] * num / den;
		p_b = (uint32_t)GAMMA_LUT[STRIP_LEDs[i].b] * num / den;


#ifdef IS_RGBW
		//Subtract white values while in linear space
		p_w = min(p_r, min(p_g, p_b));
		p_r -= p_w;
		p_g -= p_w;
		p_b -= p_w;


		p_w = p_w * CORRECTION_W / 255;
		if (p_w > 255) p_w = 255;
#endif

		//p_r = p_r * CORRECTION_R / 255;
		if (p_r > 255) p_r = 255;

		//p_g = p_g * CORRECTION_G / 255;
		if (p_g > 255) p_g = 255;

		//p_b = p_b * CORRECTION_B / 255;
		if (p_b > 255) p_b = 255;


#ifdef IS_RGBW
		FinalStrip.SetPixelColor(i, RgbwColor(p_r, p_g, p_b, p_w));
#else
		FinalStrip.SetPixelColor(i, RgbColor(p_r, p_g, p_b));
#endif

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
	FrameTime = (1000.0 / FPS) - 1;
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