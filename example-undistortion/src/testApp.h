#pragma once

#include "ofMain.h"
#include "ofxCv.h"
using namespace ofxCv;
using namespace cv;

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	
	ofVideoGrabber cam;
	ofImage undistorted;
	ofPixels previous;
	ofPixels diff;
	float diffMean;
	
	float lastTime;
	bool active;
	
	Calibration calibration;
};
