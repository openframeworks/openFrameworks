#pragma once

#include "ofMain.h"
#include "ofxiPhoneExtras.h"

class testApp : public ofxiPhoneApp, ofxiPhoneMapKitListener{
	
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
	
        // font for writing latitude longitude info
        ofTrueTypeFont font;
	
        // instance of ofxiPhoneMapKit
        // all MapKit related functionality is through this object
        ofxiPhoneMapKit mapKit;
	
        // optional callbacks for Map related events
        void regionWillChange(bool animated);
        void regionDidChange(bool animated);
        void willStartLoadingMap();
        void didFinishLoadingMap();
        void errorLoadingMap(string errorDescription);
};

