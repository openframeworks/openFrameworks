#pragma once

#include "ofConstants.h"

class ofFpsCounter {
public:
	ofFpsCounter();
	ofFpsCounter(double targetFps);
	void newFrame();
	double getFps() const;
	unsigned int getNumFrames() const;
	unsigned long long getLastFrameNanos() const;
	double getLastFrameSecs() const;

private:
	unsigned int nFrameCount;
	uint64_t secsThen, nanosThen;
	double fps;
	unsigned int nFramesForFPS;
	uint64_t secsOneSec, nanosOneSec;
	uint64_t lastFrameTime;
};
