/* Refer to the README.md in the example's root folder for more information on usage */

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

	void audioIn(ofSoundBuffer & input);

	int	bufferSize = 512;
	int	drawCounter;
	int bufferCounter;
	float buffer[512] = { 0 };
	
	ofSoundPlayer sound;
	ofSoundStream soundStream;

};

