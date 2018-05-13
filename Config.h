#pragma once


#define DESK

#ifdef POLE1
#define SECRET_MQTT_STATE_TOPIC "ledstrip/pole1"
#define SECRET_MQTT_COMMAND_TOPIC "ledstrip/pole1/set"
#define SECRET_MQTT_NAME "pole1"

#define FIREFLIES_CHANCE 3
#define COALS_COOLING  2
#define COALS_SPARKS 2
#define FIRE_COOLING  400
#define FIRE_SPARKING 40

#define POLE
#endif

#ifdef TV_STAND
#define SECRET_MQTT_STATE_TOPIC "ledstrip/strip1"
#define SECRET_MQTT_COMMAND_TOPIC "ledstrip/strip1/set"
#define SECRET_MQTT_NAME "tv_stand"

#define FIREFLIES_CHANCE 3
#define COALS_COOLING  2
#define COALS_SPARKS 2
#define FIRE_COOLING  400
#define FIRE_SPARKING 40

#endif

#ifdef HALLWAY
#define SECRET_MQTT_STATE_TOPIC "ledstrip/strip2"
#define SECRET_MQTT_COMMAND_TOPIC "ledstrip/strip2/set"
#define SECRET_MQTT_NAME "hallway"

#define FIREFLIES_CHANCE 3
#define COALS_COOLING  2
#define COALS_SPARKS 2
#define FIRE_COOLING  400
#define FIRE_SPARKING 40

#endif

#ifdef SINK
#define SECRET_MQTT_STATE_TOPIC "ledstrip/strip3"
#define SECRET_MQTT_COMMAND_TOPIC "ledstrip/strip3/set"
#define SECRET_MQTT_NAME "sink"

#define FIREFLIES_CHANCE 3
#define COALS_COOLING  2
#define COALS_SPARKS 2
#define FIRE_COOLING  400
#define FIRE_SPARKING 40

#endif

#ifdef DESK
#define SECRET_MQTT_STATE_TOPIC "ledstrip/strip4"
#define SECRET_MQTT_COMMAND_TOPIC "ledstrip/strip4/set"
#define SECRET_MQTT_NAME "desk"

#define FIREFLIES_CHANCE 3
#define COALS_COOLING  2
#define COALS_SPARKS 2
#define FIRE_COOLING  400
#define FIRE_SPARKING 40

#endif