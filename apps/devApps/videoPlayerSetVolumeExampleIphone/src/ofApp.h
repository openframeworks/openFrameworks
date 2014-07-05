#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"

class ofApp : public ofxiPhoneApp{
	
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
    
        void playPressed();
        void pausePressed();
        void scrubBegin();
        void scrubToPosition(float position);
        void scrubEnd();
        void loadPressed();
        void unloadPressed();
        void loopOnPressed();
        void loopOffPressed();
        void nativeOnPressed();
        void nativeOffPressed();
        void muteOnPressed();
        void muteOffPressed();

	//ofVideoPlayer video;
        ofiPhoneVideoPlayer video;
};
