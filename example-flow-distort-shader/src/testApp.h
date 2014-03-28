#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "MotionAmplifier.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
    RateTimer appRate, camRate;
	ofVideoGrabber cam;
    MotionAmplifier motionAmplifier;
};