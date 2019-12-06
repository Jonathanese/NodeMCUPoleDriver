/*

	Jon Patrick Weber
	Library for non-blocking timers

*/

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
	StopWatch();							// Default stopwatch usage.

	StopWatch(bool started);				// Choose whether stopwatch is started initially.

	~StopWatch();

	void reset();							// Reset timer

	void reset(unsigned long startTime);	// Reset timer to specific value

	unsigned long getTime();				// Get current duration of timer

	bool passed(unsigned long compareTime);	// If this much time has passed, return true

	bool repeat(unsigned long compareTime);	// If this much time has passed, return true and reset the timer

	/*
	These are figurative and optional. The stopwatch is
	technically still running even after stop()
	is called.
	Instead, start() is similar to reset(), but it only resets on
	the first call. This is useful for fall-through logic where you
	don't want your timer resetting each cycle.
	*/

	bool stop();	// Turns off isRunning flag. Returns true when stop is performed. Useful for performing action once in cyclic calls

	bool start();	// Turns on isRunning flag and resets the timer. Returns true when start is performed. Useful for performing action once in cyclic calls

	bool isRunning : 1;
private:
	uint64_t time;
};

#endif