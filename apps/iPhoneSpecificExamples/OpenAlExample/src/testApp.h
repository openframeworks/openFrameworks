#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"
#include "ofxOpenALSoundPlayer.h"

class testApp : public ofxiPhoneApp {
	
	public:
		void setup();
		void update();
		void draw();

		void touchDown(ofTouchEventArgs & touch);
		void touchMoved(ofTouchEventArgs & touch);
		void touchUp(ofTouchEventArgs & touch);
		void touchDoubleTap(ofTouchEventArgs & touch);
		void touchCancelled(ofTouchEventArgs &touch);
		
		void exit();
		
		void lostFocus();
		void gotFocus();
		void gotMemoryWarning();
		void deviceOrientationChanged(int newOrientation);
		
		ofxOpenALSoundPlayer synth[10]; //load in 10 instances so that they can be played multiple times (sort of). Right now ofxALSoundPlayer doesn't work with multiPlay
		int lastSoundPlayed; //counter to keep track of which sound we're playing
		
		ofPoint audioLoc[5]; // one for each possible touch ID
		int audioSize[5];
	
};
