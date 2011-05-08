#pragma once

#include "ofMain.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	vector<ofImage*> jpg8, png8, png16, exrFloat;
};
