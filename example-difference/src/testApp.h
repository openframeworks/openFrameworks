#pragma once

#include "ofMain.h"
#include "ofxCv.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	ofVideoGrabber cam;
	ofPixels previous;
	ofImage diff;
	
	// a scalar is like an ofVec4f but normally used for storing color information
	cv::Scalar diffMean;
};
