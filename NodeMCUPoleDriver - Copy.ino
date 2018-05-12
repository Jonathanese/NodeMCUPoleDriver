//These must be defined before FastLED.h

#define FASTLED_INTERRUPT_RETRY_COUNT 0
//#define FASTLED_ALLOW_INTERRUPTS 0		// May interfere with WiFi
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <FastLED.h>
#include "secrets.h"
//#include <ESP8266mDNS.h>
//#include <WiFiUdp.h>
//#include <ArduinoOTA.h>

#pragma region DEFINES
#define DEBUG
#define MAX_CURRENT 10000
#define COLOR_ORDER GRB      // if colors are mismatched; change this
#define LED_TYPE WS2812B
#define NUM_LEDS 240
#define DATA_PIN 5			// Seems to be D5 instead of GPIO5

#define BLEND_REPLACE 0		// Foreground overrides whatever the background had before
#define BLEND_ADD 1			// Foreground adds to background
#define BLEND_ALPHA 2		// Foreground interpolates between the background and foreground

const int BUFFER_SIZE = JSON_OBJECT_SIZE(10);
#define MQTT_MAX_PACKET_SIZE 512

#pragma endregion

#define SENSORNAME "strip4"
const char* light_state_topic = "ledstrip/strip4";
const char* light_set_topic = "ledstrip/strip4/set";

#pragma region GLOBAL DATA
struct BGLED
{
	CRGB COLOR;
};
struct FGLED
{
	CRGB COLOR;
	int ALPHA;
	uint8_t BLEND;
};

unsigned long currentTime;
unsigned int frameTime = 25;
unsigned int t_frameTime = 2;

int param[NUM_LEDS];
CRGB STRIP_LEDs[NUM_LEDS];
BGLED BGLEDS[NUM_LEDS];
FGLED FGLEDS[NUM_LEDS];

CRGB ROOT_COLOR = CRGB(128, 0, 0);
CRGB FG_COLOR = CRGB(255, 255, 0);
CRGB OLD_COLOR = CRGB::Black;
uint8_t brightness = 64;
uint8_t old_brightness = 255;
bool stateOn = true;
uint8_t transition = 0;
uint8_t transitionBrightness = 0;
uint8_t stored_brightness = 64;
#pragma endregion

#pragma region COMMUNICATIONS

/************ WIFI and MQTT Information (CHANGE THESE FOR YOUR SETUP) ******************/
WiFiClient espClient;
PubSubClient client(espClient);

const char* ssid = SECRET_SSID; //type your WIFI information inside the quotes
const char* password = SECRET_WIFIPWD;
const char* mqtt_server = SECRET_MQTT_SERVER;
const char* mqtt_username = SECRET_MQTT_ID;
const char* mqtt_password = SECRET_MQTT_PWD;
const int mqtt_port = 1883;

const char* on_cmd = "ON";
const char* off_cmd = "OFF";
const char* effect = "solid";
String effectString = "solid";

void setup_wifi() {
	delay(10);
	// We start by connecting to a WiFi network
#ifdef DEBUG
	Serial.println();
	Serial.print(F("Connecting to "));
	Serial.println(ssid);
#endif

	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
#ifdef DEBUG
		Serial.println(".");
#endif
	}
#ifdef DEBUG
	Serial.println("");
	Serial.println("WiFi connected");
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());
#endif
}

/*
SAMPLE PAYLOAD:
{
"brightness": 120,
"color": {"r": 255,"g": 100,"b": 100},
"state": "ON"
}
*/
void callback(char* topic, byte* payload, unsigned int length) {
	char* message = new char[length + 1];

	for (int i = 0; i < length; i++) {
		message[i] = (char)payload[i];
	}
	message[length] = '\0';

#ifdef DEBUG
	Serial.print(F("Message arrived ["));
	Serial.print(topic);
	Serial.print("] ");
	Serial.println(message);
#endif

	if (!processJson(message)) {
		return;
	}
	Serial.println(effect);
	sendState();
	delete message;
}
bool processJson(char* message) {
	StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(message);

	if (!root.success()) {
		Serial.println("parseObject() failed");
		return false;
	}

	if (root.containsKey("state")) {
		if (strcmp(root["state"], on_cmd) == 0) {
			if (!stateOn)
			{
				stateOn = true;
				transitionBrightness = 255; //Initiate a transition
				brightness = old_brightness;
				old_brightness = 0;
			}
		}
		else if (strcmp(root["state"], off_cmd) == 0) {
			if (stateOn)
			{
				stateOn = false;
				transitionBrightness = 255; //Initiate a transition
				old_brightness = brightness;
				brightness = 0;
			}
		}
	}

	if (root.containsKey("color")) {
		transition = 255; // Initiate a transition
		OLD_COLOR = ROOT_COLOR;
		ROOT_COLOR = CRGB(root["color"]["r"], root["color"]["g"], root["color"]["b"]);
	}

	if (root.containsKey(F("brightness"))) {
		transitionBrightness = 255; //Initiate a transition
		old_brightness = brightness;
		brightness = root[F("brightness")];
	}

	if (root.containsKey(F("effect"))) {
		effect = root[F("effect")];
		ChangeEffect(effect);
	}

	return true;
}
void sendState() {
	StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;

	JsonObject& root = jsonBuffer.createObject();

	root["state"] = (stateOn) ? on_cmd : off_cmd;
	JsonObject& color = root.createNestedObject("color");
	color["r"] = ROOT_COLOR.r;
	color["g"] = ROOT_COLOR.g;
	color["b"] = ROOT_COLOR.b;

	root["brightness"] = brightness;
	root["effect"] = effectString.c_str();

	char* buffer = new char[root.measureLength() + 1];
	root.printTo(buffer, root.measureLength() + 1);

	client.publish(light_state_topic, buffer, true);

#ifdef DEBUG
	Serial.print("- Send State: ");
	Serial.println(buffer);
#endif

	delete buffer;
}
void reconnect() {
	// Loop until we're reconnected
	while (!client.connected()) {
#ifdef DEBUG
		Serial.print(F("Attempting MQTT connection..."));
#endif

		// Attempt to connect
		if (client.connect(SENSORNAME, mqtt_username, mqtt_password)) {
#ifdef DEBUG
			Serial.println(F("connected"));
#endif
			client.subscribe(light_set_topic);
			sendState();
		}
		else {
#ifdef DEBUG
			Serial.print(F("failed, rc="));
			Serial.print(client.state());
			Serial.println(F(" try again in 5 seconds"));
#endif
			// Wait 5 seconds before retrying
			delay(5000);
		}
	}
}

#pragma endregion

void(*FOREGROUND)();
void(*BACKGROUND)();

void setup()
{
#ifdef DEBUG
	Serial.begin(921600);
#endif
	FOREGROUND = FG_NONE;
	BACKGROUND = BG_FLAT;
	FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_CURRENT);
	FastLED.setCorrection(TypicalPixelString);
	FastLED.setBrightness(brightness);
	FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(STRIP_LEDs, NUM_LEDS);
	setup_wifi();
	client.setServer(mqtt_server, mqtt_port);
	client.setCallback(callback);
}

unsigned long Index;

#ifdef DEBUG
unsigned int loopCounter;
unsigned long loopMillis;
#endif

void loop()
{
	currentTime = millis();
	if (!client.connected()) {
		reconnect();
	}

	if (WiFi.status() != WL_CONNECTED) {
		delay(1);
		Serial.print("WIFI Disconnected. Attempting reconnection.");
		setup_wifi();
		return;
	}
	client.loop();

	if (transition || transitionBrightness)
	{
		TransitionBrightness();
		TransitionColor();
		FastLED.show();
		while (millis() - currentTime < t_frameTime); //Run transition at different frame rate
	}
	else
	{
		BACKGROUND();
		FOREGROUND();
		FlattenAndShow();
		while (millis() - currentTime < frameTime);
	}

#ifdef DEBUG_TIMING
	if (loopCounter == 100)
	{
		loopCounter = 0;
		Serial.print("Loop Time");
		Serial.print((millis() - loopMillis) / 100);
		Serial.println("ms");
		loopMillis = millis();
	}
	else
	{
		loopCounter++;
	}
#endif
}

bool Roll(unsigned int chance)
{
	return (random16(10000) < chance);
}

void FlattenAndShow()
{
	for (int i = 0; i < NUM_LEDS; i++)
	{
		switch (FGLEDS[i].BLEND)
		{
		case BLEND_ALPHA:
			STRIP_LEDs[i] = CRGB(lerp8by8(BGLEDS[i].COLOR.r, FGLEDS[i].COLOR.r, FGLEDS[i].ALPHA),
				lerp8by8(BGLEDS[i].COLOR.g, FGLEDS[i].COLOR.g, FGLEDS[i].ALPHA),
				lerp8by8(BGLEDS[i].COLOR.b, FGLEDS[i].COLOR.b, FGLEDS[i].ALPHA));
			break;
		case BLEND_ADD:
			STRIP_LEDs[i] = CRGB(qadd8(BGLEDS[i].COLOR.r, FGLEDS[i].COLOR.r),
				qadd8(BGLEDS[i].COLOR.g, FGLEDS[i].COLOR.g),
				qadd8(BGLEDS[i].COLOR.b, FGLEDS[i].COLOR.b));
			break;
		default:
			STRIP_LEDs[i] = FGLEDS[i].COLOR;
		}
	}
	FastLED.show();
}
void TransitionColor()
{
	if (transition)
	{
		for (int i = 0; i < NUM_LEDS; i++)
		{
			STRIP_LEDs[i] = CRGB(lerp8by8(ROOT_COLOR.r, OLD_COLOR.r, transition), // Note: These transitions travel in reverse
				lerp8by8(ROOT_COLOR.g, OLD_COLOR.g, transition),
				lerp8by8(ROOT_COLOR.b, OLD_COLOR.b, transition));
		}
		transition--;
	}
}
void TransitionBrightness()
{
	if (transitionBrightness)
	{
		FastLED.setBrightness(lerp8by8(brightness, old_brightness, transitionBrightness));
		transitionBrightness--;
	}
}
void ChangeEffect(String neweffect)
{
	FOREGROUND = FG_NONE;
	if (neweffect == "lightning")
	{
		FOREGROUND = FG_LIGHTNING;
		FG_COLOR = CRGB(255, 255, 255);
		frameTime = 50;
	}
	if (neweffect == "fireflies")
	{
		FOREGROUND = FG_FIREFLIES;
		FG_COLOR = CRGB(255, 255, 0);
		frameTime = 25;
	}
}

#pragma region BACKGROUNDS

void BG_FLAT()
{
	for (int i = 0; i < NUM_LEDS; i++)
	{
		BGLEDS[i].COLOR = ROOT_COLOR;
	}
}

#pragma endregion
#pragma region FOREGROUNDS

void FG_NONE()
{
	for (int i = 0; i < NUM_LEDS; i++)
	{
		FGLEDS[i].BLEND = BLEND_ADD;
		FGLEDS[i].COLOR = CRGB::Black;
	}
}

void FG_FIREFLIES()
{
	//TODO: Consider reversing the function of param so it increases instead of decreases. This may allow use of an unsigned variable.

	for (int i = 0; i < NUM_LEDS; i++)
	{
		FGLEDS[i].COLOR = FG_COLOR;
		FGLEDS[i].BLEND = BLEND_ALPHA;

		if (param[i] <= 0)
		{
			if (Roll(1))
			{
				param[i] = 255; //Create firefly
			}
			else
			{
				FGLEDS[i].ALPHA = 0; //Don't create firefly
			}
		}
		else
		{
			FGLEDS[i].ALPHA = 255 - cos8(param[i]); //We are using param to control the alpha channel via a function. (No idea why cosine instead of sine)
		}
		param[i] -= 4;
		if (param[i] < 0) { param[i] = 0; } //param must currently be signed in order to do sub-zero checks.
	}
}

void FG_LIGHTNING()
{
	//TODO: Consider reversing the function of param so it increases instead of decreases. This may allow use of an unsigned variable.
	if (param[0] <= 0)
	{
		if (Roll(50))
		{
			param[0] = 255; //Create lightning
		}
	}
	else
	{
		FGLEDS[0].ALPHA = Roll(2000) ? random8(5)*param[0]/5 : 0;
	}

	for (int i = 0; i < NUM_LEDS; i++)
	{
		FGLEDS[i].COLOR = FG_COLOR;
		FGLEDS[i].BLEND = BLEND_ALPHA;

		if (param[0] <= 0)
		{
			FGLEDS[i].ALPHA = 0; //Don't create lightning
		}
		else
		{
			FGLEDS[i].ALPHA = FGLEDS[0].ALPHA; //We are using param to control the alpha channel via a function.
		}
	}
	param[0] -= 10;
	if (param[0] < 0) { param[0] = 0; } //param must currently be signed in order to do sub-zero checks.

}

#pragma endregion