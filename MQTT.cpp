#include "MQTT.h"

WiFiClient espClient;
PubSubClient client(espClient);

void MQTT_Setup(MQTT_CALLBACK_SIGNATURE)
{
	WiFi_Setup();
	client.setServer(SECRET_MQTT_SERVER, SECRET_MQTT_PORT);
	client.setCallback(callback);
}

void MQTT_Loop()
{
	ArduinoOTA.handle();

	if (!client.connected()) {
		reconnect();
	}

	if (WiFi.status() != WL_CONNECTED) {
		delay(1);
		DebugMessage(DM_ERROR, "WIFI Disconnected. Attempting reconnection.");
		WiFi_Setup();
		return;
	}
	client.loop();
}

void reconnect() {
	// Loop until we're reconnected
	while (!client.connected()) {
		DebugMessage(DM_INFO, "Connecting to MQTT");

		// Attempt to connect
		if (client.connect(SECRET_MQTT_NAME, SECRET_MQTT_ID, SECRET_MQTT_PWD)) {
			DebugMessage(DM_INFO, "Connected to MQTT Server");
			client.subscribe(SECRET_MQTT_COMMAND_TOPIC);
		}
		else {
			DebugMessage(DM_ERROR, "Retry in 5s");
			// Wait 5 seconds before retrying
			delay(5000);
		}
	}
}

void publish(char* buffer)
{
	client.publish(SECRET_MQTT_STATE_TOPIC, buffer, true);
}