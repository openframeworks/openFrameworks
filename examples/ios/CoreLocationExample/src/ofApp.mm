#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(225, 225, 225);

	// initialize the accelerometer
	ofxAccelerometer.setup();
	
	coreLocation = new ofxiOSCoreLocation();
	hasCompass = coreLocation->startHeading();
	hasGPS = coreLocation->startLocation();
	
	arrowImg.loadImage("arrowLong.png");
	compassImg.loadImage("compass.png");
	
	compassImg.setAnchorPoint(160, 200);
	arrowImg.setAnchorPercent(0.5, 1.0);
	
	heading = 0.0;
}


//--------------------------------------------------------------
void ofApp::update(){	
	heading = ofLerpDegrees(heading, -coreLocation->getTrueHeading(), 0.7);
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(54);
	ofDrawBitmapString("Core Location Example", 8, 20);

	ofEnableAlphaBlending();	
	ofSetColor(255);
		ofPushMatrix();
		ofTranslate(160, 220, 0);
		ofRotateZ(heading);
		compassImg.draw(0,0);
	ofPopMatrix();
	
	ofSetColor(255);
	arrowImg.draw(160, 220);	

	ofSetColor(54);
	ofDrawBitmapString("LAT: ", 8, ofGetHeight() - 8);
	ofDrawBitmapString("LON: ", ofGetWidth() - 108, ofGetHeight() - 8);

	if(hasGPS){
		cout<<coreLocation->getLatitude()<<" | "<< coreLocation->getLatitude() <<endl;
		
		ofSetHexColor(0x009d88);
		ofDrawBitmapString(ofToString(coreLocation->getLatitude()), 8 + 33, ofGetHeight() - 8);

		ofSetHexColor(0x0f7941d);
		ofDrawBitmapString(ofToString(coreLocation->getLongitude()), (ofGetWidth() - 108) + 33, ofGetHeight() - 8);
		
	}
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchCancelled(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void ofApp::lostFocus(){
    
}

//--------------------------------------------------------------
void ofApp::gotFocus(){
    
}

//--------------------------------------------------------------
void ofApp::gotMemoryWarning(){
    
}

//--------------------------------------------------------------
void ofApp::deviceOrientationChanged(int newOrientation){
    
}
