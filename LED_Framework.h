#pragma once

#define NEOPIXELBUS

#include <FastLED.h>
#include "StopWatch.h"
#include <NeoPixelBus.h>
#include "Config.h"
#include "Color_Correction.h"
#include <vector>
#include "DebugMessage.h"

#define COLOR_ORDER GRB
#define LED_TYPE WS2812B

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
	LED* LEDs;
	uint8_t size;
#ifdef RGBW
	NeoPixelBus<NeoGrbwFeature, Neo800KbpsMethod>* strip;
#else
	NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>* strip;
#endif
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
void AdvancedShow();
void BasicShow();

void setBrightness(uint8_t newBright);
void setFrameRate(float FPS);

void SOLID();
void setEffect(void(*NewEffect)(), String EffString);
String getEffect();
