#pragma once

#include "ofMain.h"
#include "ofxCv.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	ofVideoGrabber cam;
	ofPixels previous;
	ofImage diff;
	cv::Mat columnMean;
};
