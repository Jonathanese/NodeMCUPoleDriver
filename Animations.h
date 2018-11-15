// Animations.h

#ifndef _ANIMATIONS_h
#define _ANIMATIONS_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "LED_Framework.h"
#include "ArduinoJson.h"
#include "MQTT.h"

void ChangeEffect(String effect);
bool processJson(char* message);
void callback(char* topic, byte* payload, unsigned int length);
void sendState();

void TEST_RUN();
void TEST_BORDER();
void FIRE();
void COALS();
void FIREFLIES();
void LIGHTNING();

#endif
