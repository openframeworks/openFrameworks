#pragma once

#include "ofMain.h"
#include "ofxiOSExtras.h"

class ofApp : public ofxiOSApp, ofxiOSMapKitListener{
	
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
	
        // instance of ofxiOSMapKit
        // all MapKit related functionality is through this object
        ofxiOSMapKit mapKit;
	
        // optional callbacks for Map related events
        void regionWillChange(bool animated);
        void regionDidChange(bool animated);
        void willStartLoadingMap();
        void didFinishLoadingMap();
        void errorLoadingMap(string errorDescription);
};

