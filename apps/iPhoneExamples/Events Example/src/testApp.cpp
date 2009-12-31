
#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){	
	// initialize the accelerometer
	ofxAccelerometer.setup();
	
	// touch events will be sent to testApp
	ofxMultiTouch.addListener(this);
	
	//iPhoneAlerts will be sent to this.
	ofxiPhoneAlerts.addListener(this);
	
	ofBackground(50,50,50);
	
    enabled=false;
	
	// add the newFloat method to listen for eventsAddon newFloatEvent
	ofAddListener(evObject.newFloatEvent,this,&testApp::newFloat);
	
	// add the newFloat method to listen for eventsAddon newFloatEvent
	ofAddListener(evObject.newIntEvent,this,&testApp::newInt);
	
}


//--------------------------------------------------------------
void testApp::update() {
	ofNotifyEvent( ofEvents.update, voidEventArgs);
}

//--------------------------------------------------------------
void testApp::draw() {
	ofDrawBitmapString(floatMsg,20,20);
	ofDrawBitmapString(intMsg,20,40);
	ofDrawBitmapString("click to enable/disable events",20,60);
}

//--------------------------------------------------------------
void testApp::newFloat(float & f){
	floatMsg 	= "newFloat event:  " + ofToString(f);
	cout << floatMsg << "\n";
}

//--------------------------------------------------------------
void testApp::newInt(int & i){
	intMsg	 	= "newInt   event:  " + ofToString(i);
}

void testApp::exit() {
}


//--------------------------------------------------------------
void testApp::touchDown(float x, float y, int touchId, ofxMultiTouchCustomData *data){
	if(enabled)
        evObject.disable();
    else
        evObject.enable();
	
    enabled=!enabled;	
}

//--------------------------------------------------------------
void testApp::touchMoved(float x, float y, int touchId, ofxMultiTouchCustomData *data){
}

//--------------------------------------------------------------
void testApp::touchUp(float x, float y, int touchId, ofxMultiTouchCustomData *data){
}

//--------------------------------------------------------------
void testApp::touchDoubleTap(float x, float y, int touchId, ofxMultiTouchCustomData *data){
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

