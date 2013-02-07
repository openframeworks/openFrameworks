#pragma once

#include "ofMain.h"
#include "ofxAutoControlPanel.h"
#include "ofxCv.h"

#include "imatrix.h"
#include "ETF.h"
#include "fdog.h"
#include "myvec.h"

class testApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();
	
	int width, height;
	vector<ofImage> input, output, canny;
	
	imatrix img;
	ETF etf;
	
	ofxAutoControlPanel gui;
};
