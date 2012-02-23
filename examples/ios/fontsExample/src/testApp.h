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
	void touchCancelled(ofTouchEventArgs &touch);
	
	bool bFirst;
	string typeStr;
	
	ofTrueTypeFont  franklinBook14;
	ofTrueTypeFont	verdana14;
	ofTrueTypeFont	verdana30;
	
	ofTrueTypeFont  franklinBook14A;
	ofTrueTypeFont	verdana14A;
	
	ofPoint touchPt;
	ofPoint drag;
	bool bDragging;
	
};


