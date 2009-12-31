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
	
	void touchDown(float x, float y, int touchId, ofxMultiTouchCustomData *data);
	void touchMoved(float x, float y, int touchId, ofxMultiTouchCustomData *data);
	void touchUp(float x, float y, int touchId, ofxMultiTouchCustomData *data);
	void touchDoubleTap(float x, float y, int touchId, ofxMultiTouchCustomData *data);
	
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

