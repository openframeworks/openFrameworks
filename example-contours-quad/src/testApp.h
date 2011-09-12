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
	void mousePressed(int x, int y, int button);
	void keyPressed(int key);
	
	ofVideoGrabber cam;	
	ContourFinder contourFinder;
	vector< vector<cv::Point> > quads;
	float threshold;
	ofColor targetColor;
	ofImage unwarped;
};
