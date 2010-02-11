#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"

class testApp : public ofxiPhoneApp {
	
public:
	void setup();
	void update();
	void draw();

	void touchDown(ofTouchEventArgs &touch);
	void touchMoved(ofTouchEventArgs &touch);
	void touchUp(ofTouchEventArgs &touch);
	void touchDoubleTap(ofTouchEventArgs &touch);
	
	void exit();
	
	void lostFocus();
	void gotFocus();
	void gotMemoryWarning();
	
	ofxiPhoneCoreLocation * coreLocation;
	
	bool hasCompass;
	bool hasGPS;
};
