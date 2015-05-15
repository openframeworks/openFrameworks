#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCv.h"

class ofApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();
	
	vector<ofImage> input, output, canny;
	
    ofParameter<float> sigma1, sigma2, tau;
    ofParameter<int> halfw, smoothPasses, black, thresh, cannyParam1, cannyParam2;
    ofParameter<bool> doFDoG, doThresh, doThin, doCanny;
    
	ofxPanel gui;
};
