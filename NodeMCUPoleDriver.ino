
#include "Animations.h"



void setup()
{
	Serial.begin(921600);
	DM_MASK = DM_ERROR + DM_INFO + DM_SEND + DM_RECEIVE + DM_TIMING;
	MQTT_Setup(callback);
	LEDSetup();
}


void loop()
{
	MQTT_Loop();
	LEDLoop();
}



