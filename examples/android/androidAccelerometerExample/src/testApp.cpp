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
	sprintf(buf[0],"g(x) = %0.2f",accel.x);
	sprintf(buf[1],"g(y) = %0.2f",accel.y);
	sprintf(buf[2],"g(z) = %0.2f",accel.z);
	normAccel = accel.getNormalized();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(255,255,0);
	font.drawString(buf[0],10,24);
	ofSetColor(255,0,255);
	font.drawString(buf[1],10,font.stringHeight(buf[0])+24);

	ofTranslate(ofGetWidth()/2, ofGetHeight()/2);

	ofSetColor(255,255,0);
	ofLine(0,0,normAccel.x*ofGetWidth()/2,0);
	ofSetColor(255,0,255);
	ofLine(0,0,0,-normAccel.y*ofGetHeight()/2);
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
