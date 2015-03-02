#pragma once

#include "ofMain.h"
#include "ofxCv.h"

class ofApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();
	
	ofVideoGrabber camera;
	
	ofxCv::FlowFarneback farneback;
	ofxCv::FlowPyrLK pyrLk;
	
	ofxCv::Flow* curFlow;
		
    ofxGui gui;
};

