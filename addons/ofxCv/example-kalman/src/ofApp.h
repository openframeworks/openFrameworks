#pragma once

#include "ofMain.h"
#include "ofxCv.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	ofxCv::KalmanPosition kalman;
	
	ofMesh predicted, line, estimated;
	ofVec2f point;
	float speed;
};
