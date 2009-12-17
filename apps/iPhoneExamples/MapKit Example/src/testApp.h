#pragma once

#include "ofMain.h"
#include "ofxiPhoneExtras.h"

class testApp : public ofxiPhoneApp, ofxiPhoneMapKitListener {
	
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
	
	
	// font for writing latitude longitude info
	ofTrueTypeFont font;

	
	// instance of ofxiPhoneMapKit
	// all MapKit related functionality is through this object
	ofxiPhoneMapKit mapKit;
	
	
	// optional callbacks for Map related events
	void regionWillChange(bool animated);
	void regionDidChange(bool animated);
	void willStartLoadingMap();
	void didFinishLoadingMap();
	void errorLoadingMap(string errorDescription);
};

