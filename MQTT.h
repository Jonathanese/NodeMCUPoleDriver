// MQTT.h

#ifndef _MQTT_h
#define _MQTT_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "WiFi.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define MQTT_MAX_PACKET_SIZE 512
const uint8_t BUFFER_SIZE = JSON_OBJECT_SIZE(10);

void MQTT_Setup(MQTT_CALLBACK_SIGNATURE);
void MQTT_Loop();
void reconnect();
void callback(char* topic, byte* payload, unsigned int length);
void publish(char* buffer);
#endif
