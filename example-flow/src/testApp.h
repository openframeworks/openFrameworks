#pragma once

#include "ofMain.h"

#include "ofxAutoControlPanel.h"

#include "ofxCv.h"
using namespace ofxCv;
using namespace cv;

class testApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();
	
	ofVideoGrabber camera;
	
	ofImage prev, next;
	
	Mat flow;
	
	vector<Point2f> prevPts, nextPts;
	
	ofxAutoControlPanel panel;
};

