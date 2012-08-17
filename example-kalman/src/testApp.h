#pragma once

#include "ofMain.h"
#include "ofxCv.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	cv::KalmanFilter KF;
	cv::Mat_<float> measurement;
	
	ofPolyline predicted, line;
	ofVec2f point;
};
