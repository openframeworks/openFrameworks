#pragma once

#include "ofxiOS.h"

class ofApp : public ofxiOSApp {

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

		// we will have a dynaimic number of images, based on the content of a director:

		int nImages;
		ofImage	* images;
        ofDirectory DIR;

        int currentImage;

};
