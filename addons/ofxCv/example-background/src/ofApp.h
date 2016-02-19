#pragma once

#include "ofMain.h"
#include "ofxCv.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	
	ofVideoGrabber cam;
	ofxCv::RunningBackground background;
	ofImage thresholded;
};
