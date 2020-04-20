#pragma once

#define POLE1

#ifdef POLE1
#define SECRET_MQTT_STATE_TOPIC "ledstrip/pole1"
#define SECRET_MQTT_COMMAND_TOPIC "ledstrip/pole1/set"
#define SECRET_MQTT_NAME "pole1"

#define FIREFLIES_CHANCE 2
#define FIREFLIES_SPEED 20
#define FIREFLIES_FPS 25

#define LIGHTNING_CHANCE 4
#define LIGHTNING_FPS 25

#define COALS_COOLING  600
#define COALS_SPARKS 2
#define COALS_SPARK_HEAT 10000
#define COALS_FPS 25

#define FIRE_COOLING  400
#define FIRE_SPARKING 40
#define FIRE_FPS 25

#define NONE_FPS 4

#define POLE

#define STRIP_CONFIG Strips[4]{{ 0, 58 },{ 116,59 },{ 117, 174 },{ 232, 175 }};
#endif

#ifdef STRIP1
#define SECRET_MQTT_STATE_TOPIC "ledstrip/strip1"
#define SECRET_MQTT_COMMAND_TOPIC "ledstrip/strip1/set"
#define SECRET_MQTT_NAME "strip1"

#define FIREFLIES_CHANCE 2
#define FIREFLIES_SPEED 20
#define FIREFLIES_FPS 15

#define LIGHTNING_CHANCE 10
#define LIGHTNING_FPS 30

#define COALS_COOLING  600
#define COALS_SPARKS 2
#define COALS_SPARK_HEAT 10000
#define COALS_FPS 15

#define FIRE_COOLING  400
#define FIRE_SPARKING 40
#define FIRE_FPS 30

#define NONE_FPS 4

#define MAX_CURRENT 2000
#define NUM_LEDS 60
#define DATA_PIN 5

#define STRIP_CONFIG Strips[2]{{ 29, 0 },{ 30, 59 }};

#endif

#ifdef STRIP2
#define SECRET_MQTT_STATE_TOPIC "ledstrip/strip2"
#define SECRET_MQTT_COMMAND_TOPIC "ledstrip/strip2/set"
#define SECRET_MQTT_NAME "strip2"

#define FIREFLIES_CHANCE 2
#define FIREFLIES_SPEED 20
#define FIREFLIES_FPS 25

#define LIGHTNING_CHANCE 10
#define LIGHTNING_FPS 25

#define COALS_COOLING  600
#define COALS_SPARKS 2
#define COALS_SPARK_HEAT 10000
#define COALS_FPS 25

#define FIRE_COOLING  400
#define FIRE_SPARKING 40
#define FIRE_FPS 25

#define NONE_FPS 4

#define CEILING

//#define STRIP_CONFIG Strips[2]{{ 29, 0 },{ 30, 59 }};
#define STRIP_CONFIG Strips[1]{{ 0, 240 }};

#endif

#ifdef STRIP3
#define SECRET_MQTT_STATE_TOPIC "ledstrip/strip3"
#define SECRET_MQTT_COMMAND_TOPIC "ledstrip/strip3/set"
#define SECRET_MQTT_NAME "strip3"

#define FIREFLIES_CHANCE 2
#define FIREFLIES_SPEED 20
#define FIREFLIES_FPS 15

#define LIGHTNING_CHANCE 10
#define LIGHTNING_FPS 30

#define COALS_COOLING  600
#define COALS_SPARKS 2
#define COALS_SPARK_HEAT 10000
#define COALS_FPS 15

#define FIRE_COOLING  400
#define FIRE_SPARKING 40
#define FIRE_FPS 30

#define NONE_FPS 4

#define STRIP_CONFIG Strips[2]{{ 29, 0 },{ 30, 59 }};

#endif

#ifdef STRIP4
#define SECRET_MQTT_STATE_TOPIC "ledstrip/strip4"
#define SECRET_MQTT_COMMAND_TOPIC "ledstrip/strip4/set"
#define SECRET_MQTT_NAME "strip4"

#define FIREFLIES_CHANCE 2
#define FIREFLIES_SPEED 20
#define FIREFLIES_FPS 15

#define LIGHTNING_CHANCE 10
#define LIGHTNING_FPS 30

#define COALS_COOLING  600
#define COALS_SPARKS 2
#define COALS_SPARK_HEAT 10000
#define COALS_FPS 15

#define FIRE_COOLING  400
#define FIRE_SPARKING 40
#define FIRE_FPS 30

#define NONE_FPS 4

#define STRIP_CONFIG Strips[2]{{ 29, 0 },{ 30, 59 }};

#endif