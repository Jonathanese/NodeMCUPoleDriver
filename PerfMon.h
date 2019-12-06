// PerfMon.h
/*
	Created:	4/22/2019 8:20:48 PM
	Author:     Jonathan Weber
	Description: Library for monitoring performance characteristics
*/
#ifndef _PERFMON_h
#define _PERFMON_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "DebugMessage.h"
#include "StopWatch.h"

class PerfMon
{
public:
	PerfMon(Debug* debugport, uint32_t reportTime);
	~PerfMon();
	void handler();
	void Reset();
private:
	uint32_t frames;
	uint32_t reportTime = 1000;
	StopWatch FPSCheckTimer;
	Debug* itsPort;
};

#endif
