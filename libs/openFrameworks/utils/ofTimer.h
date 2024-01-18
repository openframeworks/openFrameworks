#pragma once

#include "ofUtils.h"
#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>

using namespace std::chrono;
using namespace std::chrono_literals;

class ofTimerFps {
public:
	ofTimerFps(){
		reset();
	};

	using space = std::chrono::duration<long long, std::micro>;
	space interval;
	time_point<steady_clock> wakeTime;
	
	// FIXME: double? float?
	void setPerSecond(int fps) {
		interval = duration_cast<microseconds>(1s) / fps;
	}

	void reset() {
		wakeTime = steady_clock::now();
	}
	
	void waitNext(){
		std::this_thread::sleep_until(wakeTime);
		wakeTime += interval;
	}
};


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
	void setPeriodicEvent(std::chrono::nanoseconds t);
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

