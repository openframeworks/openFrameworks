#pragma once

#include "ofMain.h"

#include <atomic>

class ofApp : public ofBaseApp {
public:
	void setup() override;
	void draw() override;

private:
	ofURLFileLoader loader;
	std::atomic<float> progress{ 0.0f };
	bool finished = false;
	int status = 0;
};
