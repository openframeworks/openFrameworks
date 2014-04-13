#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "MotionAmplifier.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	ofVideoGrabber cam;
    MotionAmplifier motionAmplifier;
};