#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"

class testApp : public ofxiPhoneApp {
	
public:
	void setup();
	void update();
	void draw();
	
	void touchDown(float x, float y, int touchId, ofxMultiTouchCustomData *data);
	void touchMoved(float x, float y, int touchId, ofxMultiTouchCustomData *data);
	void touchUp(float x, float y, int touchId, ofxMultiTouchCustomData *data);
	void touchDoubleTap(float x, float y, int touchId, ofxMultiTouchCustomData *data);
	
	void exit();
	
	void lostFocus();
	void gotFocus();
	void gotMemoryWarning();
	
	ofImage bikers;
	ofImage gears;
	ofImage tdf;
	ofImage tdfSmall;
	ofImage transparency;
	ofImage bikeIcon;	
};
