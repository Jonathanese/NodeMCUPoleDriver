#include "MQTT.h"
#include "StopWatch.h"

WiFiClient espClient;
PubSubClient client(espClient);
StopWatch reconnectTimer;

void MQTT_Setup(MQTT_CALLBACK_SIGNATURE)
{
	WiFi_Setup();
	client.setServer(SECRET_MQTT_SERVER, SECRET_MQTT_PORT);
	client.setCallback(callback);
}

void MQTT_Loop()
{
	while (WiFi.status() != WL_CONNECTED) {
		DB.Message(DM_ERROR, "WIFI Disconnected. Attempting reconnection.");
		WiFi_Setup();
		return;
	}
	ArduinoOTA.handle();
	reconnect();
	client.loop();
}

void reconnect() {
	// Attempt to connect
	if (!client.connected()) {
		if (reconnectTimer.repeat(1000))
		{
			if (client.connect(SECRET_MQTT_NAME, SECRET_MQTT_ID, SECRET_MQTT_PWD))
			{
				DB.Message(DM_INFO, "Connected to MQTT Server");
				client.subscribe(SECRET_MQTT_COMMAND_TOPIC);
			}
		}
	}
}

void publish(const char* buffer)
{
	client.publish(SECRET_MQTT_STATE_TOPIC, buffer, true);
}