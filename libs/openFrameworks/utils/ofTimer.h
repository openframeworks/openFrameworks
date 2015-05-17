#pragma once
#include "ofConstants.h"

class ofTimer {
public:
	ofTimer();
	void reset();
	void setPeriodicEvent(uint64_t nanoseconds);
	void waitNext();
private:
	void calculateNextPeriod();
#if defined(TARGET_WIN32)
	LARGE_INTEGER nextWakeTime;
	HANDLE hTimer;
#else
	timespec nextWakeTime;
#endif
	uint64_t nanosPerPeriod;
};

