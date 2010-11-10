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
		float rad = 100;
		float ringRad = rad + 10;
	
		//compass 3GS only:
		float cx = cos(ofDegToRad(-90 + coreLocation->getTrueHeading()) ) * rad;
		float cy = sin(ofDegToRad(-90 + coreLocation->getTrueHeading()) ) * rad;
		float x = ofGetWidth()/2;
		float y = ofGetHeight()/2;
		ofSetColor(255,255,255);
		ofLine(x,y,x+cx,y+cy);
		
		ofPushStyle();
			ofSetCircleResolution(64);
			ofNoFill();
			ofCircle(x, y, ringRad);
		ofPopStyle();
		
		ofDrawBitmapString("N", x, y - ringRad-10);
		ofDrawBitmapString("S", x, y + ringRad+10);
		ofDrawBitmapString("E", x + ringRad+10, y);
		ofDrawBitmapString("W", x - ringRad-10, y);
		
		cout<<coreLocation->getTrueHeading()<<" | ";

		ofDrawBitmapString("heading:" + ofToString(coreLocation->getTrueHeading(), 2), 10, 20 );
		
	}
	if(hasGPS){
		cout<<coreLocation->getLatitude()<<" | "<<coreLocation->getLongitude()<<endl;
		ofDrawBitmapString("lat:" + ofToString(coreLocation->getLatitude(), 4) + " long: " + ofToString(coreLocation->getLongitude(), 4), 10, 40);
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


