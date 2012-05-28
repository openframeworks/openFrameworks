#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"
#include "ofxiPhoneExternalDisplay.h"

class testApp : public ofxiPhoneApp, public ofxiPhoneExternalDisplay {
	
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


