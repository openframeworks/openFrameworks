#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"
#include "eventsObject.h"

class testApp : public ofxiPhoneApp {
	
public:
	void setup();
	void update();
	void draw();
	void exit();
	
	void touchDown(ofTouchEventArgs &touch);
	void touchMoved(ofTouchEventArgs &touch);
	void touchUp(ofTouchEventArgs &touch);
	void touchDoubleTap(ofTouchEventArgs &touch);
	
	
	void lostFocus();
	void gotFocus();
	void gotMemoryWarning();
	
	//method that will receive events from ofxEventsAddon
	void newFloat(float & f);
	void newInt(int & i);
	
	
	eventsObject evObject;
	ofEventArgs					voidEventArgs;

	string floatMsg;
	string intMsg;
	string expireMsg;
	
	bool enabled;
	
};

