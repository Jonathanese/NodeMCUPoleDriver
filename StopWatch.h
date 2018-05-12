// StopWatch.h

#ifndef _STOPWATCH_h
#define _STOPWATCH_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class StopWatch
{
public:
	StopWatch(); //Default stopwatch usage.
	StopWatch(bool started); //choose whether stopwatch is started initially.
	~StopWatch();
	void reset();
	void reset(uint32_t startTime);
	uint32_t getTime();

	/*
	These are figurative and optional. The stopwatch is
	technically still running even after stop()
	is called.
	Instead, start() is similar to reset(), but it only resets on
	the first call. This is useful for fall-through logic where you
	don't want your timer resetting each cycle.
	*/
	void stop();
	void start();
	bool isRunning : 1;
private:

	uint32_t time;
};

#endif
