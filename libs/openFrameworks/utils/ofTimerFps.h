// Written by Dimitre Lima in 2024

#pragma once

#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>

using namespace std::chrono;
using namespace std::chrono_literals;

class ofTimerFps {
public:
	ofTimerFps();
	void setFps(int fps);
	void reset();
	void waitNext();

	using space = std::chrono::duration<long long, std::nano>;
	space interval;
	time_point<steady_clock> wakeTime;
	time_point<steady_clock> lastWakeTime;
	
};
