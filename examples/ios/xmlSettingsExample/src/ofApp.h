#pragma once

#include "ofxiOS.h"
#include "ofxXmlSettings.h"

#define NUM_PTS 800

class ofApp : public ofxiOSApp{

	public:

        void setup();
        void update();
        void draw();
        void exit();
    
        void touchDown(ofTouchEventArgs & touch);
        void touchMoved(ofTouchEventArgs & touch);
        void touchUp(ofTouchEventArgs & touch);
        void touchDoubleTap(ofTouchEventArgs & touch);
        void touchCancelled(ofTouchEventArgs & touch);
	
        void lostFocus();
        void gotFocus();
        void gotMemoryWarning();
        void deviceOrientationChanged(int newOrientation);

		ofxXmlSettings XML;
		ofTrueTypeFont TTF;

		string xmlStructure;
		string message;

		ofPoint dragPts[NUM_PTS];

		int pointCount;
		int lineCount;
		int lastTagNumber;

		float red;
		float green;
		float blue;
};

