#pragma once

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
	unsigned long long secsThen, nanosThen;
	double fps;
	unsigned int nFramesForFPS;
	unsigned long long secsOneSec, nanosOneSec;
	unsigned long long lastFrameTime;
};
