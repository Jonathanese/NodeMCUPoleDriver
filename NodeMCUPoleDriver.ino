
#include "Animations.h"
#include "PerfMon.h"

PerfMon PM(&DB, 30000);

void setup()
{
	DB.begin();
	MQTT_Setup(callback);
	LEDSetup();
}


void loop()
{
	MQTT_Loop();
	LEDLoop();
	PM.handler();
}



