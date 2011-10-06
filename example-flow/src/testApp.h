#pragma once

#include "ofMain.h"
#include "ofxAutoControlPanel.h"
#include "ofxCv.h"

class testApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();
	
	ofVideoGrabber camera;
	
	ofImage prev, next;
	
	cv::Mat flow;
	
	vector<cv::Point2f> prevPts, nextPts;
	
	ofxAutoControlPanel panel;
};

