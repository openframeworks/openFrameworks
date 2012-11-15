#pragma once

#include "ofMain.h"

class testApp : public ofxiPhoneApp{
	
    public:
        void setup();
        void update();
        void draw();
        void exit();
	
    void mousePressed(int x, int y, int button);

		ofTrueTypeFont font;
        ofPoint mousePoint;
};

