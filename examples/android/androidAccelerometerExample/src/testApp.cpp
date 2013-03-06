#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	// initialize the accelerometer
	ofxAccelerometer.setup();

	font.loadFont("verdana.ttf",24);
	ofSetLineWidth(10);
	ofBackground(0,0,0);
}

//--------------------------------------------------------------
void testApp::update(){
	accel = ofxAccelerometer.getForce();
	messages[0] = "g(x) = " + ofToString(accel.x,2);
	messages[1] = "g(y) = " + ofToString(accel.y,2);
	messages[2] = "g(z) = " + ofToString(accel.z,2);
	normAccel = accel.getNormalized();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(255,255,0);
	font.drawString(messages[0],10,font.stringHeight(messages[0])+20);
	ofSetColor(255,0,255);
	font.drawString(messages[1],10,(font.stringHeight(messages[0])+20)*2);
	ofSetColor(0,255,255);
	font.drawString(messages[2],10,(font.stringHeight(messages[0])+20)*3);

	ofPushMatrix();
	ofTranslate(ofGetWidth()/2, ofGetHeight()/2);

	ofSetColor(255,255,0);
	ofLine(0,0,normAccel.x*ofGetWidth()/2,0);
	ofSetColor(255,0,255);
	ofLine(0,0,0,-normAccel.y*ofGetHeight()/2);
	// we don't draw z as the perspective might be confusing
	// but it's approximately one when the device is still and parallel
	// to the ground
	ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::touchDown(int x, int y, int id){

}

//--------------------------------------------------------------
void testApp::touchMoved(int x, int y, int id){

}

//--------------------------------------------------------------
void testApp::touchUp(int x, int y, int id){

}

//--------------------------------------------------------------
void testApp::touchDoubleTap(int x, int y, int id){

}

//--------------------------------------------------------------
void testApp::touchCancelled(int x, int y, int id){

}

//--------------------------------------------------------------
void testApp::swipe(ofxAndroidSwipeDir swipeDir, int id){

}

//--------------------------------------------------------------
void testApp::pause(){

}

//--------------------------------------------------------------
void testApp::stop(){

}

//--------------------------------------------------------------
void testApp::resume(){

}

//--------------------------------------------------------------
void testApp::reloadTextures(){

}

//--------------------------------------------------------------
bool testApp::backPressed(){
	return false;
}

//--------------------------------------------------------------
void testApp::okPressed(){

}

//--------------------------------------------------------------
void testApp::cancelPressed(){

}
