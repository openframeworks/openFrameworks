#pragma once
#include "ofConstants.h"

class ofTimer {
public:
	ofTimer();
	void reset();
	void setPeriodicEvent(unsigned long long nanoseconds);
	void waitNext();
private:
	void calculateNextPeriod();
#if (defined(TARGET_LINUX) && !defined(TARGET_RASPBERRY_PI))
	timespec nextWakeTime;
#elif defined(TARGET_WIN32)
	LARGE_INTEGER nextWakeTime;
	HANDLE hTimer;
#else
	unsigned long long   nextWakeTimeSecs;
	unsigned long long   nextWakeTimeNanos;
#endif
	unsigned long long nanosPerPeriod;
};

