//
//
//

#include "Animations.h"
#include "ArduinoJson.h"

void callback(char* topic, byte* payload, unsigned int length)
{
	char* message = new char[length + 1];

	for (uint8_t i = 0; i < length; i++) {
		message[i] = (char)payload[i];
	}
	message[length] = '\0';

	DB.Message(DM_RECEIVE, "[" + String(topic) + "]: " + String(message));

	processJson(message);

	delete message;

	sendState();
}

uint8_t brightnessState = 0;

bool processJson(char* message) {
	StaticJsonDocument<BUFFER_SIZE> root;

	auto error = deserializeJson(root, message);

	if (error) {
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
		brightnessState = brightness;
		setBrightness(brightness);
	}

	if (root.containsKey(F("effect"))) {
		ChangeEffect(root[F("effect")]);
	}

	return true;
}

void sendState() {
	StaticJsonDocument<BUFFER_SIZE> root;

	root["state"] = (stateOn) ? "ON" : "OFF";
	JsonObject color = root.createNestedObject("color");
	color["r"] = Foreground.r;
	color["g"] = Foreground.g;
	color["b"] = Foreground.b;

	root["brightness"] = brightness;
	root["effect"] = getEffect();

	//This part is dumb. I have to use a string for SerializeJson, but need a const char* for publish

	String buffer;

	serializeJson(root, buffer);

	char* charbuffer = new char[buffer.length() + 1];

	buffer.toCharArray(charbuffer, buffer.length() + 1);

	publish(charbuffer);

	delete charbuffer;
}

void ChangeEffect(String effect)
{
	if (effect.equals("lightning"))
	{
		setEffect(LIGHTNING, "lightning");
		setFrameRate(LIGHTNING_FPS);
		return;
	}
	if (effect.equals("fire"))
	{
		setEffect(FIRE, "fire");
		setFrameRate(FIRE_FPS);
		return;
	}
	if (effect.equals("coals"))
	{
		setEffect(COALS, "coals");
		setFrameRate(COALS_FPS);
		return;
	}
	if (effect.equals("fireflies"))
	{
		setEffect(FIREFLIES, "fireflies");
		setFrameRate(FIREFLIES_FPS);
		return;
	}
	if (effect.equals("solid"))
	{
		setEffect(SOLID, "solid");
		setFrameRate(NONE_FPS);
		return;
	}
	if (effect.equals("none"))
	{
		setEffect(SOLID, "none");
		setFrameRate(NONE_FPS);
		return;
	}
	if (effect.equals("testborder"))
	{
		setEffect(TEST_BORDER, "testborder");
		setFrameRate(NONE_FPS);
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

		// Step 3.  Randomly ignite new sparks of heat near the bottom
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
		for (uint8_t i = 0; i < Strips[s].size; i++) {
			//if (Roll(COALS_COOLING))
			//{
			Strips[s].LEDs[i].param -= random16(COALS_COOLING); //= qsub8(Strips[s].LEDs[i].param, Roll(COALS_COOLING) && Roll(COALS_COOLING));
	//}
		}

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
			Strips[s].LEDs[y].param += random16(COALS_SPARK_HEAT);
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
		for (uint8_t l = 0; l < Strips[s].size; l++)
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
				Strips[s].LEDs[l].param -= FIREFLIES_SPEED;
			}

			Strips[s].setColor(l, Foreground.lerp8(CRGB::Black, 240).lerp8(CRGB::Yellow, 255 - cos8(Strips[s].LEDs[l].param)));
		}
	}
}

void LIGHTNING()
{
	static int16_t PARAM = 0;
	static uint8 val = 0;
	//TODO: Consider reversing the function of param so it increases instead of decreases. This may allow use of an unsigned variable.
	if (PARAM <= 0)
	{
		if (Roll(LIGHTNING_CHANCE))
		{
			PARAM = 255; //Create lightning
		}
		setBrightness(brightnessState);
	}
	else
	{
		val = Roll(1000) ? random8(8) * PARAM / 8 : 0;
		setBrightness(brightnessState + (val * (0xff - brightnessState)));
	}
	for (uint8_t s = 0; s < numStrips; s++)
	{
		for (uint8_t p = 0; p < Strips[s].size; p++)
		{
			Strips[s].setColor(p, Foreground.lerp8(CRGB::White, val));
		}
	}

	PARAM -= 10;
	if (PARAM < 0) { PARAM = 0; } //param must currently be signed in order to do sub-zero checks.
}