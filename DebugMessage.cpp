#include "DebugMessage.h"

uint8_t DM_MASK;

void DebugMessage(uint8_t DM_TYPE, String message)
{
	switch (DM_TYPE)
	{
	case DM_SEND:
		if (!(DM_MASK & DM_SEND)) return;
		message = "< " + message;
		message.replace("\n", "\n< ");
		break;
	case DM_RECEIVE:
		if (!(DM_MASK & DM_RECEIVE)) return;
		message = "> " + message;
		message.replace("\n", "\n> ");
		break;
	case DM_ERROR:
		if (!(DM_MASK & DM_ERROR)) return;
		message = "! " + message;
		message.replace("\n", "\n! ");
		break;
	case DM_INFO:
		if (!(DM_MASK & DM_INFO)) return;
		message = "- " + message;
		message.replace("\n", "\n- ");
		break;
	case DM_TIMING:
		if (!(DM_MASK & DM_TIMING)) return;
		message = "~ " + message;
		message.replace("\n", "\n~ ");
		break;
	}

	Serial.println(message);
}