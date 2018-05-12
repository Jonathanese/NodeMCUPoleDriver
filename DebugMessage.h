// DebugMessage.h

#ifndef _DEBUGMESSAGE_h
#define _DEBUGMESSAGE_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define DM_NONE 0
#define DM_SEND 1
#define DM_RECEIVE 2
#define DM_ERROR 4
#define DM_INFO 8
#define DM_TIMING 16

extern uint8_t DM_MASK;

void DebugMessage(uint8_t DM_TYPE, String message);

#endif
