#pragma once

// ofTime only
#include "ofUtils.h"
#include <queue>


class ofFpsCounter {
public:
	ofFpsCounter();
	ofFpsCounter(double targetFps);
	void newFrame();

	// no need to call it usually, useful if
	// the update rate of the source is not very
	// consistent. calling update from the main
	// loop for example will make the measurement
	// update more often
	void update();
	double getFps() const;
	uint64_t getNumFrames() const;
	uint64_t getLastFrameNanos() const;
	double getLastFrameSecs() const;
	uint64_t getLastFrameFilteredNanos() const;
	double getLastFrameFilteredSecs() const;
	void setFilterAlpha(float alpha);

private:
	void update(double now);
	uint64_t nFrameCount;
	ofTime then;
	double fps;
	std::chrono::nanoseconds lastFrameTime;
	std::chrono::nanoseconds filteredTime;
	double filterAlpha;
	std::queue<double> timestamps;
};





#include <chrono>
using namespace std::chrono;
using namespace std::chrono_literals;

struct fpsCounterX {
public:
	int nAverages = 20;
	using space = std::chrono::duration<long double, std::nano>;
	time_point<steady_clock> lastTick;
	steady_clock::duration onesec = 1s;
	std::vector <space> intervals;
	space interval;
	space average;
	bool firstTick = true;
	int cursor = 0;

	void tick() {
		if (firstTick) {
			firstTick = false;
			lastTick = steady_clock::now();
			return;
		}

		interval = steady_clock::now() - lastTick;
		lastTick = steady_clock::now();
		if (intervals.size() < nAverages) {
			intervals.emplace_back(interval);
		} else {
			intervals[cursor] = interval;
			cursor = (cursor+1)%nAverages;
		}
	}
	
	double getFps()  {
		average = std::reduce(intervals.begin(), intervals.end());
		return (double)intervals.size() * onesec / average;
	}
	
	float get() {
		average = std::reduce(intervals.begin(), intervals.end());
		return (float)intervals.size() * onesec / average;
	}
};
