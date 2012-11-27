#pragma once

#include "ofMain.h"

class testApp : public ofBaseApp {

    public:
        void setup();
        void update();
        void draw();
        void exit();
	
      void mouseMoved(int x, int y);

		ofTrueTypeFont font;
        ofPoint mousePoint;
};

