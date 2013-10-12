#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	
	// initialize the accelerometer
	ofxAccelerometer.setup();

	// initialize all of the Ball particles
	for(int i=0; i<NUM_POINTS; i++) balls[i].init();
}

//--------------------------------------------------------------
void ofApp::update() {
	for(int i=0; i<NUM_POINTS; i++) balls[i].update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	for(int i=0; i<NUM_POINTS; i++) balls[i].draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::touchDown(int x, int y, int id){
	//toggle the touched state of the ball
	balls[id].touched = true;
	balls[id].moveTo(x, y);
}

//--------------------------------------------------------------
void ofApp::touchMoved(int x, int y, int id){
	balls[id].moveTo(x, y);
}

//--------------------------------------------------------------
void ofApp::touchUp(int x, int y, int id){
	//toggles off the touched state of the ball
	balls[id].touched = false;
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

