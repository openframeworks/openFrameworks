#pragma once

#include "ofxiOS.h"

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

        ofTexture texGray;
        ofTexture texColor;
        ofTexture texColorAlpha;

        int w;
        int h;
		
        unsigned char * colorPixels;
        unsigned char * grayPixels;
        unsigned char * colorAlphaPixels;
};


