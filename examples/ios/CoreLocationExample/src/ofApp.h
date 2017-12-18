#pragma once

#include "ofxiOS.h"
#include "ofxiOSCoreLocation.h"
#include "ofxiOSAccelerometer.h"

class ofApp : public ofxiOSApp{
	
    public:
        ofApp();
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
	
        ofxiOSCoreLocation * coreLocation;
	
        float heading;
	
        bool hasCompass;
        bool hasGPS;
	
        ofImage arrowImg;
        ofImage compassImg;
};
