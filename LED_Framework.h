// Animation.h

#ifndef _LED_FRAMEWORK_h
#define _LED_FRAMEWORK_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "secrets.h"

//These must be defined before FastLED.h

#define FASTLED_INTERRUPT_RETRY_COUNT 0
//#define FASTLED_ALLOW_INTERRUPTS 0		// May interfere with WiFi

#include <FastLED.h>
#include "StopWatch.h"
#include <vector>

#define MAX_CURRENT 10000
#define COLOR_ORDER GRB
#define LED_TYPE WS2812B
#define NUM_LEDS 240
#define DATA_PIN 7

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

uint8_t stripWraparound(int idx);

void LEDSetup();
void LEDLoop();

void setBrightness(uint8_t newBright);

void SOLID();
void setEffect(void(*NewEffect)(), String EffString);
String getEffect();

#endif
