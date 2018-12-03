#pragma once


#define HALLWAY

#ifdef TEST
#define SECRET_MQTT_STATE_TOPIC "ledstrip/TEST"
#define SECRET_MQTT_COMMAND_TOPIC "ledstrip/TEST/set"
#define SECRET_MQTT_NAME "TEST"

#define FIREFLIES_CHANCE 1
#define LIGHTNING_CHANCE 3
#define COALS_COOLING  600
#define COALS_SPARKS 2
#define COALS_SPARK_HEAT 10000
#define FIRE_COOLING  400
#define FIRE_SPARKING 40

#define POLE
#endif

#ifdef POLE1
#define SECRET_MQTT_STATE_TOPIC "ledstrip/pole1"
#define SECRET_MQTT_COMMAND_TOPIC "ledstrip/pole1/set"
#define SECRET_MQTT_NAME "pole1"

#define FIREFLIES_CHANCE 1
#define LIGHTNING_CHANCE 4
#define COALS_COOLING  600
#define COALS_SPARKS 2
#define COALS_SPARK_HEAT 10000
#define FIRE_COOLING  400
#define FIRE_SPARKING 40

#define POLE
#define STRIP_CONFIG Strips[4]{{ 3, 57 },{ 116,62 },{ 121, 175 },{ 232, 178 }};
#endif

#ifdef TV_STAND
#define SECRET_MQTT_STATE_TOPIC "ledstrip/strip1"
#define SECRET_MQTT_COMMAND_TOPIC "ledstrip/strip1/set"
#define SECRET_MQTT_NAME "tv_stand"

#define FIREFLIES_CHANCE 2
#define LIGHTNING_CHANCE 10
#define COALS_COOLING  600
#define COALS_SPARKS 2
#define COALS_SPARK_HEAT 10000
#define FIRE_COOLING  400
#define FIRE_SPARKING 40

#define STRIP_CONFIG Strips[2]{{ 29, 0 },{ 30, 59 }};

#endif

#ifdef HALLWAY
#define SECRET_MQTT_STATE_TOPIC "ledstrip/strip2"
#define SECRET_MQTT_COMMAND_TOPIC "ledstrip/strip2/set"
#define SECRET_MQTT_NAME "hallway"

#define FIREFLIES_CHANCE 2
#define LIGHTNING_CHANCE 10
#define COALS_COOLING  600
#define COALS_SPARKS 2
#define COALS_SPARK_HEAT 10000
#define FIRE_COOLING  500
#define FIRE_SPARKING 50

//#define STRIP_CONFIG Strips[2]{{ 29, 0 },{ 30, 59 }};
#define STRIP_CONFIG Strips[1]{{ 0, 59 }};

#endif

#ifdef SINK
#define SECRET_MQTT_STATE_TOPIC "ledstrip/strip3"
#define SECRET_MQTT_COMMAND_TOPIC "ledstrip/strip3/set"
#define SECRET_MQTT_NAME "sink"

#define FIREFLIES_CHANCE 2
#define LIGHTNING_CHANCE 10
#define COALS_COOLING  600
#define COALS_SPARKS 2
#define COALS_SPARK_HEAT 10000
#define FIRE_COOLING  400
#define FIRE_SPARKING 40

#define STRIP_CONFIG Strips[2]{{ 29, 0 },{ 30, 59 }};

#endif

#ifdef DESK
#define SECRET_MQTT_STATE_TOPIC "ledstrip/strip4"
#define SECRET_MQTT_COMMAND_TOPIC "ledstrip/strip4/set"
#define SECRET_MQTT_NAME "desk"

#define FIREFLIES_CHANCE 2
#define LIGHTNING_CHANCE 10
#define COALS_COOLING  600
#define COALS_SPARKS 2
#define COALS_SPARK_HEAT 10000
#define FIRE_COOLING  400
#define FIRE_SPARKING 40

#define STRIP_CONFIG Strips[2]{{ 29, 0 },{ 30, 59 }};

#endif