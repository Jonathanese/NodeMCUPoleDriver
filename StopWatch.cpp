//
//
//

#include "StopWatch.h"

StopWatch::StopWatch()
{
	reset();
	start();
}

StopWatch::StopWatch(bool started)
{
	reset();
	started ? start() : stop();
}

StopWatch::~StopWatch()
{
}

void StopWatch::reset()
{
	time = millis();
}

void StopWatch::reset(uint32_t startTime)
{
	time = millis() - startTime;
}

uint32_t StopWatch::getTime()
{
	return millis() - time;
}

void StopWatch::start()
{
	if (!isRunning)
	{
		isRunning = true;
		reset();
	}
}

void StopWatch::stop()
{
	if (isRunning)
	{
		isRunning = false;
	}
}