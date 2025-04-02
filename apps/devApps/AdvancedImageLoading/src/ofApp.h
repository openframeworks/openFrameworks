#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	vector<ofImage*> jpg8;
	vector<ofImage *> jpg2000;
	vector<ofImage*> gif8;
	vector<ofImage*> png8;
	vector<ofShortImage*> png16;
	vector<ofShortImage *> png32;
	vector<ofShortImage *> png48;
	vector<ofFloatImage*> exrFloat;

	ofImage img8;
	ofShortImage img16;
	ofShortImage img32;
	ofShortImage img48;
	ofFloatImage imgf;
};
