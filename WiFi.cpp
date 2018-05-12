#include "WiFi.h"

void WiFi_Setup() {
	delay(10);
	// We start by connecting to a WiFi network
	DebugMessage(DM_INFO, "Connecting to " + String(SECRET_SSID));

	WiFi.hostname(SECRET_MQTT_NAME);
	ArduinoOTA.setHostname(SECRET_MQTT_NAME);
	WiFi.begin(SECRET_SSID, SECRET_WIFIPWD);

	while (WiFi.waitForConnectResult() != WL_CONNECTED) {
		DebugMessage(DM_ERROR, "Connection Failed! Rebooting...");
		delay(5000);
		ESP.restart();
	}

	DebugMessage(DM_INFO, "Connected As " + WiFi.localIP().toString());

	ArduinoOTA.onStart([]() {
		DebugMessage(DM_INFO, "Start");
	});
	ArduinoOTA.onEnd([]() {
		DebugMessage(DM_INFO, "End");
	});
	ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
		DebugMessage(DM_INFO, String(progress / (total / 100)));
	});
	ArduinoOTA.onError([](ota_error_t error) {
		if (error == OTA_AUTH_ERROR) DebugMessage(DM_ERROR, "Auth Failed");
		else if (error == OTA_BEGIN_ERROR) DebugMessage(DM_ERROR, "Begin Failed");
		else if (error == OTA_RECEIVE_ERROR) DebugMessage(DM_ERROR, "Receive Failed");
		else if (error == OTA_END_ERROR) DebugMessage(DM_ERROR, "End Failed");
	});
	ArduinoOTA.begin();
	DebugMessage(DM_INFO, "Ready");
}