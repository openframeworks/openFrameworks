#pragma once

#include "ofMain.h"
#include "ofxAndroid.h"

class ofApp : public ofxAndroidApp{
	
	public:
		
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void windowResized(int w, int h);

		void touchDown(int x, int y, int id);
		void touchMoved(int x, int y, int id);
		void touchUp(int x, int y, int id);
		void touchDoubleTap(int x, int y, int id);
		void touchCancelled(int x, int y, int id);
		void swipe(ofxAndroidSwipeDir swipeDir, int id);
		void deviceOrientationChanged(ofOrientation newOrientation);

		void pause();
		void stop();
		void resume();
		void reloadTextures();

		bool backPressed();
		void okPressed();
		void cancelPressed();

		ofVideoGrabber grabber;

		// Image storing a clone of the grabber image
		ofImage grabberImage;

		int one_second_time;
		int camera_fps;
		int frames_one_sec;

		bool cameraFacingFront;
		int cameraOrientation;

		int appOrientation;

};
