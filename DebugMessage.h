/*
	Jonathan P Weber
	Library for creating formatted and filtered debug outputs

	Modified for ESP8266

*/

// DebugMessage.h

#ifndef _DEBUGMESSAGE_h
#define _DEBUGMESSAGE_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define DM_SEND 1
#define DM_RECEIVE 2
#define DM_SSEND 4
#define DM_SRECEIVE 8
#define DM_ERROR 16
#define DM_INFO 32
#define DM_TIMING 64

#define DM_INIT_SD 128
#define DM_WRITE_SD 256
#define DM_SD_MANUAL_FLUSH 512

#define SD_CS 43

class Debug
{
public:
	Debug(uint32_t baud, uint16 mask);
	~Debug();
	void begin();
	void Message(uint16 DM_TYPE, String message);
	void Message(uint16 DM_TYPE, String message, bool newline);

	bool SD_IsInserted = true;

private:

	struct {
		bool SEND : 1;
		bool RECEIVE : 1;
		bool SSEND : 1;
		bool SRECEIVE : 1;
		bool ERROR : 1;
		bool INFO : 1;
		bool TIMING : 1;
		bool INIT_SD : 1;
		bool WRITE_SD : 1;
		bool SD_MANUAL_FLUSH : 1;
	}ConfigMask;

	uint32_t BaudRate;
};

extern Debug DB;

#endif
