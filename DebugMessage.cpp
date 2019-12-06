/*
	Jonathan P Weber
	Library for creating formatted and filtered debug outputs

*/

#include "DebugMessage.h"

#define ANSI_NONE ""
#define ANSI_GRAY "\033[01;30m"
#define ANSI_GREEN_FAINT "\033[02;32m"
#define ANSI_BLINK_RED "\033[01;05;31m"
#define ANSI_RED "\033[01;31m"
#define ANSI_GREEN "\033[01;32m"
#define ANSI_YELLOW "\033[01;33m"
#define ANSI_YELLOW_FAINT "\033[02;33m"
#define ANSI_BLUE "\033[01;34m"
#define ANSI_BLUE_FAINT "\033[02;34m"
#define ANSI_URL "\033[01;04;34m"
#define ANSI_MAGENTA "\033[01;35m"
#define ANSI_CYAN "\033[01;36m"
#define ANSI_WHITE "\033[01;37m"
#define ANSI_RESET "\033[00;49m"
#define ANSI_RESET_BLINK "\033[01;25m"
#define ANSI_CLI ANSI_WHITE

uint8 DM_MASK;

//Instantiate global
Debug DB(115200,
	DM_ERROR
	| DM_TIMING
	| DM_INFO
	| DM_RECEIVE
	| DM_SEND
);

Debug::Debug(uint32_t baud, uint16 mask)
{
	BaudRate = baud;
	ConfigMask.SEND = mask & DM_SEND;
	ConfigMask.RECEIVE = mask & DM_RECEIVE;
	ConfigMask.SSEND = mask & DM_SSEND;
	ConfigMask.SRECEIVE = mask & DM_SRECEIVE;
	ConfigMask.INFO = mask & DM_INFO;
	ConfigMask.TIMING = mask & DM_TIMING;
	ConfigMask.ERROR = mask & DM_ERROR;
	ConfigMask.INIT_SD = mask & DM_INIT_SD;
	ConfigMask.WRITE_SD = mask & DM_WRITE_SD;
	ConfigMask.SD_MANUAL_FLUSH = mask & DM_SD_MANUAL_FLUSH;
}

Debug::~Debug()
{
}

void Debug::begin()
{
	Serial.begin(BaudRate);
}

void Debug::Message(uint16 DM_TYPE, String message, bool newline)
{
	String ANSI_Prefix;

	switch (DM_TYPE)
	{
	case DM_SEND:
		if (!(ConfigMask.SEND)) return;
		message = "<< " + message;
		message.replace("\n", "\r\n<< ");
		ANSI_Prefix = ANSI_GREEN;
		break;
	case DM_RECEIVE:
		if (!(ConfigMask.RECEIVE)) return;
		message = ">> " + message;
		message.replace("\n", "\r\n>> ");
		ANSI_Prefix = ANSI_GREEN;
		break;
	case DM_SSEND:
		if (!(ConfigMask.SSEND)) return;
		message = "< " + message;
		message.replace("\n", "\r\n< ");
		ANSI_Prefix = ANSI_BLUE;
		break;
	case DM_SRECEIVE:
		if (!(ConfigMask.SRECEIVE)) return;
		message = "> " + message;
		message.replace("\n", "\r\n> ");
		ANSI_Prefix = ANSI_BLUE;
		break;
	case DM_ERROR:
		if (!(ConfigMask.ERROR)) return;
		message = "! " + message;
		message.replace("\n", "\r\n! ");
		ANSI_Prefix = ANSI_RED;
		break;
	case DM_INFO:
		if (!(ConfigMask.INFO)) return;
		message = "- " + message;
		message.replace("\n", "\r\n- ");
		ANSI_Prefix = ANSI_WHITE;
		break;
	case DM_TIMING:
		if (!(ConfigMask.TIMING)) return;
		message = "~ " + message;
		message.replace("\n", "\r\n~ ");
		ANSI_Prefix = ANSI_GRAY;
		break;
	}

	Serial.print(ANSI_Prefix + message + ANSI_CLI);

	if (newline)
	{
		Serial.println();
	}
}

void Debug::Message(uint16 DM_TYPE, String message)
{
	Message(DM_TYPE, message, true);
}