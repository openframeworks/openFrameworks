// Written by Dimitre Lima in 2024

#pragma once

#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>

class ofTimerFps {
public:
	ofTimerFps();
	void setFps(int fps);
	void reset();
	void waitNext();

	using space = std::chrono::duration<long long, std::nano>;
	space interval;
	std::chrono::time_point<std::chrono::steady_clock> wakeTime;
	std::chrono::time_point<std::chrono::steady_clock> lastWakeTime;
	int currentFPS = 60;
	
};
