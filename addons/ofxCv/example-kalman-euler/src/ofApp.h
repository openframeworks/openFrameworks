#pragma once

#include "ofMain.h"
#include "ofxCv.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	ofxCv::KalmanEuler kalman;
	
	ofMatrix4x4 m, mPredicted;
	ofEasyCam cam;
};
