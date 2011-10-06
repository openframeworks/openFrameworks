#pragma once

#include "ofMain.h"
#include "ofxCv.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void mousePressed(int x, int y, int button);
	void keyPressed(int key);
	
	ofVideoGrabber cam;	
	ofxCv::ContourFinder contourFinder;
	vector< vector<cv::Point> > quads;
	float threshold;
	ofColor targetColor;
	ofImage unwarped;
};
