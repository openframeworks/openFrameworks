#pragma once

#include <queue>
#include <chrono>

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
	void update(std::chrono::time_point<std::chrono::steady_clock> now);
	uint64_t nFrameCount = 0;
	double fps = 0;

	using space = std::chrono::duration<long long, std::nano>;
	std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
	std::chrono::time_point<std::chrono::steady_clock> then = std::chrono::steady_clock::now();
	space lastFrameTime;
	double filteredTime = 0;
	double filterAlpha = 0.9;
	std::queue<std::chrono::time_point<std::chrono::steady_clock>> timestamps;
};
