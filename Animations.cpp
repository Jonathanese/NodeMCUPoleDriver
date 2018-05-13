//
//
//

#include "Animations.h"

void callback(char* topic, byte* payload, unsigned int length)
{
	char* message = new char[length + 1];

	for (uint8_t i = 0; i < length; i++) {
		message[i] = (char)payload[i];
	}
	message[length] = '\0';

	DebugMessage(DM_RECEIVE, "[" + String(topic) + "]: " + String(message));

	processJson(message);

	delete message;

	sendState();
}

bool processJson(char* message) {
	StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(message);

	if (!root.success()) {
		Serial.println("parseObject() failed");
		return false;
	}

	if (root.containsKey("state")) {
		if (strcmp(root["state"], "ON") == 0) {
			if (!stateOn)
			{
				stateOn = true;
			}
		}
		else if (strcmp(root["state"], "OFF") == 0) {
			if (stateOn)
			{
				stateOn = false;
				setBrightness(0);
			}
		}
	}

	if (root.containsKey("color")) {
		Foreground = CRGB(root["color"]["r"], root["color"]["g"], root["color"]["b"]);
	}

	if (root.containsKey(F("brightness"))) {
		brightness = root[F("brightness")];
		setBrightness(brightness);
	}

	if (root.containsKey(F("effect"))) {
		ChangeEffect(root[F("effect")]);
	}

	return true;
}

void sendState() {
	StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;

	JsonObject& root = jsonBuffer.createObject();

	root["state"] = (stateOn) ? "ON" : "OFF";
	JsonObject& color = root.createNestedObject("color");
	color["r"] = Foreground.r;
	color["g"] = Foreground.g;
	color["b"] = Foreground.b;

	root["brightness"] = brightness;
	root["effect"] = getEffect();

	char* buffer = new char[root.measureLength() + 1];
	root.printTo(buffer, root.measureLength() + 1);

	publish(buffer);

	delete buffer;
}

void ChangeEffect(String effect)
{
	if (effect.equals("fire"))
	{
		setEffect(FIRE, "fire");
		return;
	}
	if (effect.equals("coals"))
	{
		setEffect(COALS, "coals");
		return;
	}
	if (effect.equals("fireflies"))
	{
		setEffect(FIREFLIES, "fireflies");
		return;
	}
	if (effect.equals("solid"))
	{
		setEffect(SOLID, "solid");
		return;
	}
	if (effect.equals("none"))
	{
		setEffect(SOLID, "none");
		return;
	}
}

void TEST_RUN()
{
	static uint8_t height;
	for (int s = 0; s < numStrips; s++)
	{
		for (int h = 0; h < Strips[s].size; h++)
		{
			if (h == height)
			{
				Strips[s].setColor(h, CRGB::Blue);
			}
			else
			{
				Strips[s].setColor(h, Strips[s].getColor(h).fadeToBlackBy(1));
			}
		}
	}

	height++;
	if (height >= 60)
	{
		height = 0;
	}
}

void TEST_BORDER()
{
	for (int i = 0; i < numStrips; i++)
	{
		Strips[i].setColor(0, CRGB::Green);
		Strips[i].setColor(Strips[i].size - 1, CRGB::Red);
	}
}



#define MIDPOINT 96

CRGB fireColor(uint8_t heat, CRGB Spark, CRGB Flame)
{
	if (heat <= MIDPOINT)
	{
		return Flame.lerp8(CRGB::Black, (((MIDPOINT - heat) * 255 / MIDPOINT)));
	}
	else if (heat < 255)
	{
		return Flame.lerp8(Spark, (((heat - MIDPOINT) * 255 / (255 - MIDPOINT))));
	}
	else
	{
		return Spark;
	}
}

void FIRE()
{
	for (uint8_t s = 0; s < numStrips; s++)
	{
		// Step 1.  Cool down every cell a little
		for (uint8_t i = 0; i < Strips[s].size; i++) {
			Strips[s].LEDs[i].param = qsub8(Strips[s].LEDs[i].param, random8(0, (FIRE_COOLING / Strips[s].size) + 2));
		}

		// Step 2.  Heat from each cell drifts 'up' and diffuses a little
		for (uint8_t k = Strips[s].size - 1; k >= 2; k--)
		{
			Strips[s].LEDs[k].param = (
				(Strips[s].getParam(k - 1) * 5) +
				(Strips[s].getParam(k - 2) * 5) +
				(Strips[stripWraparound(s + 1)].getParam(k - 1))
				) / 11;
		}

		// Step 3.  Randomly ignite new 'sparks' of heat near the bottom
		if (random8() < FIRE_SPARKING)
		{
			uint8_t y = random8(2);
			Strips[s].LEDs[y].param = qadd8(Strips[s].LEDs[y].param, random8(32, 64));
		}

		// Step 4.  Map from heat cells to LED colors
		for (uint8_t j = 0; j < Strips[s].size; j++)
		{
			//CRGB color = ColorFromPalette(firecolors, Strips[s].LEDs[j].param);
			Strips[s].setColor(j, fireColor(Strips[s].LEDs[j].param, CRGB(255, 255, 0), Foreground));
		}
	}
}



void COALS()
{
	for (uint8_t s = 0; s < numStrips; s++)
	{
		// Step 1.  Cool down every cell a little
		//for (uint8_t i = 0; i < Strips[s].size; i++) {
		//	Strips[s].LEDs[i].param = qsub8(Strips[s].LEDs[i].param, Roll(COALS_COOLING)&& Roll(COALS_COOLING));
		//}

		// Step 2.  Heat from each cell drifts 'up' and diffuses a little
		for (int k = Strips[s].size - 1; k >= 0; k--)
		{
			Strips[s].LEDs[k].param = (
				Strips[s].getParam(k - 1) +
				Strips[s].getParam(k - 2) +
				Strips[s].getParam(k + 1) +
				Strips[s].getParam(k + 2) +
				Strips[stripWraparound(s + 1)].getParam(k) +
				Strips[stripWraparound(s - 1)].getParam(k)
				) / 6;
		}
		uint8_t y;
		// Step 3.  Randomly ignite new 'sparks' of heat near the bottom
		for (int k = 0; k < COALS_SPARKS; k++)
		{
			y = random8(Strips[s].size);
			Strips[s].LEDs[y].param += random16(8192);
		}

		// Step 4.  Map from heat cells to LED colors
		for (uint8_t j = 0; j < Strips[s].size; j++)
		{
			//CRGB color = ColorFromPalette(firecolors, Strips[s].LEDs[j].param);
			Strips[s].setColor(j, Foreground.lerp16(CRGB::Black, 65535 - Strips[s].getParam(j)));
		}
	}
}


void FIREFLIES()
{
	for (uint8_t s = 0; s < numStrips; s++)
	{
		for (uint8_t l= 0; l < Strips[s].size; l++)
		{
		
			if ((s < 2) && Roll(FIREFLIES_CHANCE))
			{
				Strips[s].LEDs[l].param = 255;
			}
			else if (Strips[s].LEDs[l].param > 255)
			{
				Strips[s].LEDs[l].param = 0;
			}
			else if (Strips[s].LEDs[l].param > 0)
			{
				Strips[s].LEDs[l].param -= 10;
			}
		
			Strips[s].setColor(l, Foreground.lerp8(CRGB::Black, 240).lerp8(CRGB::Yellow, 255 - cos8(Strips[s].LEDs[l].param)));
		
		}
	}
}