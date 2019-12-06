/*
	Created:	4/22/2019 8:20:48 PM
	Author:     Jonathan Weber
	Description: Library for monitoring
*/

#include "PerfMon.h"

PerfMon::PerfMon(Debug* debugport, uint32_t newReportTime)
{
	itsPort = debugport;
	reportTime = newReportTime;
}

PerfMon::~PerfMon()
{
}

void PerfMon::Reset()
{
	FPSCheckTimer.reset();
	frames = 0;
}

void PerfMon::handler()
{
	static unsigned long lastMicros;
	static unsigned long longestMicros;
	if (FPSCheckTimer.passed(reportTime))
	{
		char FPSprefix = ' ';
		char LONGprefix;
		float LongPrefixDivider = 1;
		float fps = FPSCheckTimer.getTime(); //temporary assignment to check non-zero condition
		if (fps == 0)
		{
			fps == 1;
		}
		fps = (float)frames / fps; //We will use real time instead of ideal time here. Good for lower frame rates.
		FPSCheckTimer.reset();
		fps *= 1000; //The above is frames per millisecond

		if (fps >= 1000)
		{
			if (fps >= 1000000)
			{
				FPSprefix = 'M';
				fps /= 1000000.0;
			}
			else
			{
				FPSprefix = 'k';
				fps /= 1000.0;
			}
		}

		if (longestMicros < 1000000)
		{
			if (longestMicros < 1000) // microseconds
			{
				LONGprefix = 'u';
			}
			else //milliseconds
			{
				LONGprefix = 'm';
				LongPrefixDivider = 1000;
			}
		}
		else
		{
			LongPrefixDivider = 1000000;
		}

		itsPort->Message(DM_TIMING, String(fps, 1) + FPSprefix + "FPS (Longest: " + String((double)longestMicros / LongPrefixDivider, 1) + LONGprefix + "s)      ");
		longestMicros = 0;
		frames = 0;
	}
	else
	{
		frames++;
		if ((micros() - lastMicros) > longestMicros)
		{
			longestMicros = micros() - lastMicros;
		}
		lastMicros = micros();
	}
}