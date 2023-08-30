#pragma once

// ofTime only
#include "ofUtils.h"
#include "ofConstants.h"
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
