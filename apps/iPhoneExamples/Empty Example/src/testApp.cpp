
#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){	
	// initialize the accelerometer
	ofxAccelerometer.setup();
	
	// register touch events
	ofAddListener(ofEvents.touchDown, this, &testApp::touchDown);
	ofAddListener(ofEvents.touchUp, this, &testApp::touchUp);
	ofAddListener(ofEvents.touchMoved, this, &testApp::touchMoved);
	ofAddListener(ofEvents.touchDoubleTap, this, &testApp::touchDoubleTap);
	
	//iPhoneAlerts will be sent to this.
	ofxiPhoneAlerts.addListener(this);
}

//--------------------------------------------------------------
void testApp::update() {
}

//--------------------------------------------------------------
void testApp::draw() {
}

//--------------------------------------------------------------
void testApp::exit() {
}


//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs &touch){
}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs &touch){
}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs &touch){
}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs &touch){
}


//--------------------------------------------------------------
void testApp::lostFocus() {
}

//--------------------------------------------------------------
void testApp::gotFocus() {
}

//--------------------------------------------------------------
void testApp::gotMemoryWarning() {
}

