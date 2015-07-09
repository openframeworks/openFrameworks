#pragma once

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

private:
	void update(double now);
	uint64_t nFrameCount;
	uint64_t secsThen, nanosThen;
	double fps;
	uint64_t lastFrameTime;
	queue<double> timestamps;
};
