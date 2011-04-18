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
	
	ofVideoGrabber cam;
	ofPixels previous;
	ofImage diff;
	
	// a scalar is like an ofVec4f
	// but it's normally used for storing color information
	Scalar diffMean, diffMeanMean;
};
