#pragma once

#include "ofMain.h"
#include "ofxiOS.h"
#include "ofxiOSExtras.h"
#include "ofxiOSExternalDisplay.h"

class ofApp : public ofxiOSApp, public ofxiOSExternalDisplay {
	
    public:
        void setup();
        void update();
        void draw();
        void exit();
    
        void presentExternalDisplayPopup();
        void presentExternalDisplayNotFoundPopup();
        void presentMirroringFailedPopup();
        void popupDismissed();
	
        void touchDown(ofTouchEventArgs & touch);
        void touchMoved(ofTouchEventArgs & touch);
        void touchUp(ofTouchEventArgs & touch);
        void touchDoubleTap(ofTouchEventArgs & touch);
        void touchCancelled(ofTouchEventArgs & touch);

        void lostFocus();
        void gotFocus();
        void gotMemoryWarning();
        void deviceOrientationChanged(int newOrientation);
    
        void externalDisplayConnected();
        void externalDisplayDisconnected();
        void externalDisplayChanged();
    
        ofRectangle buttonExternalDisplayRect;
        ofRectangle buttonMirrorDisplayRect;

};


