#pragma once

#include "ofMain.h"
#include "GLSmokeTestCore.h"

class ofApp : public ofBaseApp {
public:
	void setup() override;
	void draw() override;

private:
	GLSmokeTestCore core;
};
