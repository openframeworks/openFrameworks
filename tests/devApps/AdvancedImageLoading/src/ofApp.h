#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	vector<ofImage*> jpg8;
	vector<ofImage*> gif8;
	vector<ofImage*> png8;
	vector<ofShortImage*> png16;
	vector<ofFloatImage*> exrFloat;

	ofImage img8;
	ofShortImage img16;
	ofFloatImage imgf;
};
