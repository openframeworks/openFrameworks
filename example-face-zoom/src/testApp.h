#pragma once

#include "ofMain.h"
#include "ofxCv.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	ofVideoGrabber cam;
	ofImage gray, graySmall, cropped;
	
	cv::CascadeClassifier classifier;
	vector<cv::Rect> objects;
	float scaleFactor;
};
