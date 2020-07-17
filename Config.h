#pragma once

#define STRIP2

#ifdef TEST
#define SECRET_MQTT_STATE_TOPIC "ledstrip/test1"
#define SECRET_MQTT_COMMAND_TOPIC "ledstrip/test1/set"
#define SECRET_MQTT_NAME "test1"

#define FIREFLIES_CHANCE 10
#define FIREFLIES_SPEED 5
#define FIREFLIES_FPS 120

#define LIGHTNING_CHANCE 50
#define LIGHTNING_FPS 120

#define COALS_COOLING  600
#define COALS_SPARKS 2
#define COALS_SPARK_HEAT 10000
#define COALS_FPS 120

#define FIRE_COOLING  400
#define FIRE_SPARKING 40
#define FIRE_FPS 120

#define NONE_FPS 10

#define IS_RGBW

#define NUM_LEDS 20
#define STRIP_CONFIG Strips[1]{{ 0 , 19 }};
#define DITHER_COUNT 8


#endif

#ifdef SIGN1
#define SECRET_MQTT_STATE_TOPIC "ledstrip/sign1"
#define SECRET_MQTT_COMMAND_TOPIC "ledstrip/sign1/set"
#define SECRET_MQTT_NAME "sign1"

#define FIREFLIES_CHANCE 10
#define FIREFLIES_SPEED 5
#define FIREFLIES_FPS 120

#define LIGHTNING_CHANCE 50
#define LIGHTNING_FPS 120

#define COALS_COOLING  600
#define COALS_SPARKS 2
#define COALS_SPARK_HEAT 10000
#define COALS_FPS 120

#define FIRE_COOLING  400
#define FIRE_SPARKING 40
#define FIRE_FPS 120

#define NONE_FPS 10


#define NUM_LEDS 18
#define STRIP_CONFIG Strips[1]{{ 0 , 17 }};
#define DITHER_COUNT 64

//#define NUM_LEDS 60
//#define STRIP_CONFIG Strips[2]{{ 29, 0 },{ 30, 59 }};
//#define DITHER_COUNT 8

#endif

#ifdef POLE1
#define SECRET_MQTT_STATE_TOPIC "ledstrip/pole1"
#define SECRET_MQTT_COMMAND_TOPIC "ledstrip/pole1/set"
#define SECRET_MQTT_NAME "pole1"

#define FIREFLIES_CHANCE 2
#define FIREFLIES_SPEED 10
#define FIREFLIES_FPS 20

#define LIGHTNING_CHANCE 4
#define LIGHTNING_FPS 20

#define COALS_COOLING  100
#define COALS_SPARKS 2
#define COALS_SPARK_HEAT 10000
#define COALS_FPS 20

#define FIRE_COOLING  400
#define FIRE_SPARKING 40
#define FIRE_FPS 20

#define NONE_FPS 4

#define POLE
//#define NO_DITHERING
#define DITHER_COUNT 3

#define NUM_LEDS 240
#define STRIP_CONFIG Strips[4]{{ 0, 58 },{ 116,59 },{ 117, 174 },{ 232, 175 }};
#endif

#ifdef STRIP1
#define SECRET_MQTT_STATE_TOPIC "ledstrip/strip1"
#define SECRET_MQTT_COMMAND_TOPIC "ledstrip/strip1/set"
#define SECRET_MQTT_NAME "strip1"

#define FIREFLIES_CHANCE 2
#define FIREFLIES_SPEED 10
#define FIREFLIES_FPS 30

#define LIGHTNING_CHANCE 10
#define LIGHTNING_FPS 30

#define COALS_COOLING  600
#define COALS_SPARKS 2
#define COALS_SPARK_HEAT 10000
#define COALS_FPS 30

#define FIRE_COOLING  400
#define FIRE_SPARKING 40
#define FIRE_FPS 30

#define NONE_FPS 4


#define NUM_LEDS 30
#define STRIP_CONFIG Strips[1]{{ 0 , 29 }};
#define DITHER_COUNT 32

//#define NUM_LEDS 60
//#define STRIP_CONFIG Strips[2]{{ 29, 0 },{ 30, 59 }};
//#define DITHER_COUNT 8

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

#define COALS_COOLING  200 
#define COALS_SPARKS 10
#define COALS_SPARK_HEAT 5000
#define COALS_FPS 25

#define FIRE_COOLING  550
#define FIRE_SPARKING 40
#define FIRE_FPS 25

#define NONE_FPS 4
#define DITHER_COUNT 2

#define IS_RGBW

//#define STRIP_CONFIG Strips[2]{{ 29, 0 },{ 30, 59 }};
#define NUM_LEDS 240
#define STRIP_CONFIG Strips[2]{{ 119, 0 }, {120, 239}};

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