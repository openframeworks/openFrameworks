#pragma once

#include "ofMain.h"
#include "ofxCv.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	ofVideoGrabber cam;
	ofImage cropped;
	ofxCv::ObjectFinder objectFinder;
};
