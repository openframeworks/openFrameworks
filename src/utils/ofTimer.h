#pragma once
#include "ofConstants.h"
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
#if (defined(TARGET_LINUX) && !defined(TARGET_RASPBERRY_PI))
    timespec nextWakeTime;
#elif defined(TARGET_WIN32)
	LARGE_INTEGER nextWakeTime;
	HANDLE hTimer;
#else
	struct sec_ns{
	    uint64_t tv_sec;
	    uint64_t tv_nsec;
	    bool operator<(const sec_ns & time2){
	        return tv_sec < time2.tv_sec || (tv_sec == time2.tv_sec && tv_nsec < time2.tv_nsec);
	    }
	}nextWakeTime;
#endif
	uint64_t nanosPerPeriod;
};

