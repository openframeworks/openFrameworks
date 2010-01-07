
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
	
	
	ofBackground(50,50,50);
	
    enabled=false;
	
	// add the newFloat method to listen for eventsAddon newFloatEvent
	ofAddListener(evObject.newFloatEvent,this,&testApp::newFloat);
	
	// add the newFloat method to listen for eventsAddon newFloatEvent
	ofAddListener(evObject.newIntEvent,this,&testApp::newInt);
	
}


//--------------------------------------------------------------
void testApp::update() {
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


//--------------------------------------------------------------
void testApp::exit() {
}


//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs &touch){
	if(enabled)
        evObject.disable();
    else
        evObject.enable();
	
    enabled=!enabled;
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


