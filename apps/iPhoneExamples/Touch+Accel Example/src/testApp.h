
#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"
#include "Ball.h"

#define NUM_POINTS				10


class testApp : public ofxiPhoneApp {
	
public:
	void setup();
	void update();
	void draw();
	void exit();

	void touchDown(ofTouchEventArgs & touch);
	void touchMoved(ofTouchEventArgs & touch);
	void touchUp(ofTouchEventArgs & touch);
	void touchDoubleTap(ofTouchEventArgs & touch);
	
	void lostFocus();
	void gotFocus();
	void gotMemoryWarning();
	
	Ball balls[NUM_POINTS];
};
