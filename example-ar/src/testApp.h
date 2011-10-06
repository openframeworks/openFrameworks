#pragma once

#include "ofMain.h"
#include "ofxCv.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	ofVideoGrabber cam;
	ofxCv::Calibration calibration;
	vector<cv::Point3f> objectPoints;
	vector<cv::Point2f> imagePoints;
	ofMatrix4x4 model;
	bool found;
	cv::Size patternSize;
	ofLight light;
};
