#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	// register touch events
	ofRegisterTouchEvents(this);
	
	// initialize the accelerometer
	ofxAccelerometer.setup();
	
	//iPhoneAlerts will be sent to this.
	ofxiPhoneAlerts.addListener(this);
		
	ofBackground(0,0,0);

	coreLocation = new ofxiPhoneCoreLocation();
	hasCompass = coreLocation->startHeading();
	hasGPS = coreLocation->startLocation();
}


//--------------------------------------------------------------
void testApp::update(){	

}

//--------------------------------------------------------------
void testApp::draw(){

	if(hasCompass){
		//compass 3GS only:
		float cx = cos(ofDegToRad(coreLocation->getTrueHeading()) ) * 100;
		float cy = sin(ofDegToRad(coreLocation->getTrueHeading()) ) * 100;
		float x = ofGetWidth()/2;
		float y = ofGetHeight()/2;
		ofSetColor(255,255,255);
		ofLine(x,y,x+cx,y+cy);
		cout<<coreLocation->getTrueHeading()<<" | ";
	}
	if(hasGPS){
		cout<<coreLocation->getLatitude()<<" | "<<coreLocation->getLongitude()<<endl;
	}
}

//--------------------------------------------------------------
void testApp::exit(){
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

//--------------------------------------------------------------
void testApp::deviceOrientationChanged(int newOrientation){
}



//--------------------------------------------------------------
void testApp::touchCancelled(ofTouchEventArgs& args){

}

