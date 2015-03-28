#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	
	// initialize the accelerometer
	ofxAccelerometer.setup();
	
	ofSetOrientation(OF_ORIENTATION_90_RIGHT);//Set iOS to Orientation Landscape Right
	
	vagRounded.load("vag.ttf", 18);
	ofBackground(50,50,50);	
}

//--------------------------------------------------------------
void ofApp::update(){
	counter = counter + 0.033f;
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofTranslate(-60, -60, 0);

    timeString  = ofGetTimestampString("%H:%M:%S.%i") + "\n";
	
	ofSetHexColor(0xffffff);
	vagRounded.drawString(eventString, 98,198);
	
	ofSetColor(255,122,220);
	vagRounded.drawString(eventString, 100,200);
	
	ofSetHexColor(0xffffff);
	vagRounded.drawString(timeString, 98,98);
	
	ofSetColor(255,122,220);
	vagRounded.drawString(timeString, 100,100);
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){
    eventString = "touchDown = " + ofToString(touch.x, 2) + ", " + ofToString(touch.y, 2) + " - id " + ofToString(touch.id);
}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){
    eventString = "touchMoved = " + ofToString(touch.x, 2) + ", " + ofToString(touch.y, 2) + " - id " + ofToString(touch.id);
}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch){
    eventString = "touchUp = " + ofToString(touch.x, 2) + ", " + ofToString(touch.y, 2) + " - id " + ofToString(touch.id);
}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs & touch){
    eventString = "doubleTap at = " + ofToString(touch.x, 2) + ", " + ofToString(touch.y, 2);
}

//--------------------------------------------------------------
void ofApp::touchCancelled(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::lostFocus(){
    eventString = "alert - lost focus";
}

//--------------------------------------------------------------
void ofApp::gotFocus(){
    eventString = "alert - got focus";
}

//--------------------------------------------------------------
void ofApp::gotMemoryWarning(){
    eventString = "alert - memory warning";
}

//--------------------------------------------------------------
void ofApp::deviceOrientationChanged(int newOrientation){
    eventString = "alert - orientation change to " + ofToString(newOrientation);
}

