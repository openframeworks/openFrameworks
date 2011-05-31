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
	ofImage gray, graySmall;
	
	CascadeClassifier classifier;
	vector<cv::Rect> objects;
	float scaleFactor;
};
