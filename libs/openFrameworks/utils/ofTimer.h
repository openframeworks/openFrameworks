#pragma once

#include "ofConstants.h"
#include "ofUtils.h"

class ofTimer {
public:
	
	/// \brief Constructs the default ofTimer().
	ofTimer();

	/// \brief  Reset the starting instant of the periodic event to now.
	void reset();
	
	/// \brief Sets the frequency in \p nanoseconds that events happen.
	/// 
	/// The timer will begin calculating the events beginning the moment
	/// the function is called.
	/// \param nanoseconds The number of nanoseconds between events.
	void setPeriodicEvent(uint64_t nanoseconds);
	
	/// \brief Sleep this thread until the next periodic event.
	void waitNext();
private:
	void calculateNextPeriod();
	std::chrono::nanoseconds nanosPerPeriod;
#if defined(TARGET_WIN32)
	LARGE_INTEGER nextWakeTime;
	HANDLE hTimer;
#else
	ofTime nextWakeTime;
#endif
};

