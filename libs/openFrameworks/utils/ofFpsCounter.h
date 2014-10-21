#pragma once

class ofFpsCounter {
public:
	ofFpsCounter();
	ofFpsCounter(double targetFps);
	void newFrame();
	double getFps();
	unsigned int getNumFrames();
	unsigned long long getLastFrameNanos();
	double getLastFrameSecs();

private:
	unsigned int nFrameCount;
	unsigned long long secsThen, nanosThen;
	double fps;
	unsigned int nFramesForFPS;
	unsigned long long secsOneSec, nanosOneSec;
	unsigned long long lastFrameTime;
};
