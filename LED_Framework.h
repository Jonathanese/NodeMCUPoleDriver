// Animation.h

#ifndef _LED_FRAMEWORK_h
#define _LED_FRAMEWORK_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "secrets.h"
#include "Config.h"

//These must be defined before FastLED.h

//#define FASTLED_INTERRUPT_RETRY_COUNT 0
//#define FASTLED_ALLOW_INTERRUPTS 0		// May interfere with WiFi



#include <FastLED.h>
#include "StopWatch.h"
#include <vector>



#define COLOR_ORDER GRB
#define LED_TYPE WS2812B

/*

This is a stupid unnecessary extra step as for all intents and purposes, these should exist in Config.h.

But for some reason, you get a ton of errors if you don't define them RIGHT HERE.

*/

#ifdef POLE
#define MAX_CURRENT 12000
#define NUM_LEDS 240
#define DATA_PIN 7
#elif defined CEILING
#define MAX_CURRENT 12000
#define NUM_LEDS 240
#define DATA_PIN 5
#else
#define MAX_CURRENT 2000
#define NUM_LEDS 60
#define DATA_PIN 5

#endif
extern StopWatch FrameTimer;

typedef struct LED
{
	uint16_t param;
	uint8_t address;
};

class LEDStrip
{
public:
	LEDStrip(uint8_t StartAddress, uint8_t StopAddress);
	~LEDStrip();
	bool setColor(uint8_t idx, CRGB color);
	CRGB getColor(int idx);
	uint16_t getParam(int idx);
	LED *LEDs;
	uint8_t size;
};

extern bool stateOn;
extern LEDStrip Strips[];
extern CRGB Foreground;
extern uint16_t global_param;
extern uint8_t numStrips;
extern uint8_t brightness;
bool Roll(unsigned int chance);
uint8_t stripWraparound(int idx);
extern bool bypassBrightness;
void LEDSetup();
void LEDLoop();

void setBrightness(uint8_t newBright);
void setFrameRate(float FPS);

void SOLID();
void setEffect(void(*NewEffect)(), String EffString);
String getEffect();

#endif
