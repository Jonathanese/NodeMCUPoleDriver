// WiFi.h

#ifndef _WIFI_h
#define _WIFI_h

#include "arduino.h"
#include "DebugMessage.h"
#include "secrets.h"
#include "Config.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

void WiFi_Setup();

#endif
