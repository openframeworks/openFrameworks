
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
		
	keyboard = new ofxiPhoneKeyboard(0,32,300,32);
	keyboard->setVisible(true);
	keyboard->setBgColor(0, 0, 0, 255);
	keyboard->setFontColor(255,255,255,255);
	keyboard->setFontSize(26);
	
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
	if(!keyboard->isKeyboardShowing()) keyboard->openKeyboard();	
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



