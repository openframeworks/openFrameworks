#pragma once

#include "ofMain.h"
#include "ofxAutoControlPanel.h"
#include "ofxCv.h"

class testApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();
	
	vector<ofImage> input, output, canny;
	
	ofxAutoControlPanel gui;
};
