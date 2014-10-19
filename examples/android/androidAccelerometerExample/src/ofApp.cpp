#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	// initialize the accelerometer
	ofxAccelerometer.setup();

	font.load("verdana.ttf",24);
	ofSetLineWidth(10);
	ofBackground(0,0,0);
}

//--------------------------------------------------------------
void ofApp::update(){
	accel = ofxAccelerometer.getForce();
	messages[0] = "g(x) = " + ofToString(accel.x,2);
	messages[1] = "g(y) = " + ofToString(accel.y,2);
	messages[2] = "g(z) = " + ofToString(accel.z,2);
	normAccel = accel.getNormalized();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(255,255,0);
	font.drawString(messages[0],10,font.stringHeight(messages[0])+20);
	ofSetColor(255,0,255);
	font.drawString(messages[1],10,(font.stringHeight(messages[0])+20)*2);
	ofSetColor(0,255,255);
	font.drawString(messages[2],10,(font.stringHeight(messages[0])+20)*3);

	ofPushMatrix();
	ofTranslate(ofGetWidth()/2, ofGetHeight()/2);

	ofSetColor(255,255,0);
	ofDrawLine(0,0,normAccel.x*ofGetWidth()/2,0);
	ofSetColor(255,0,255);
	ofDrawLine(0,0,0,-normAccel.y*ofGetHeight()/2);
	// we don't draw z as the perspective might be confusing
	// but it's approximately one when the device is still and parallel
	// to the ground
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){ 
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::touchDown(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchMoved(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchUp(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchCancelled(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::swipe(ofxAndroidSwipeDir swipeDir, int id){

}

//--------------------------------------------------------------
void ofApp::pause(){

}

//--------------------------------------------------------------
void ofApp::stop(){

}

//--------------------------------------------------------------
void ofApp::resume(){

}

//--------------------------------------------------------------
void ofApp::reloadTextures(){

}

//--------------------------------------------------------------
bool ofApp::backPressed(){
	return false;
}

//--------------------------------------------------------------
void ofApp::okPressed(){

}

//--------------------------------------------------------------
void ofApp::cancelPressed(){

}
