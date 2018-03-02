#include "ofApp.h"
#include "ofLog.h"
#include "ofEvents.h"

static float g_rotation(3);
static ofColor color(255,255,255);

ofApp::ofApp(){
	ofLogNotice("ofApp")<<"Create";

}

ofApp::~ofApp(){
	ofLogNotice("ofApp")<<"Destroy";
}

//--------------------------------------------------------------
void ofApp::setup(){
	rotate = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
	rotate += 3;
	if(rotate >= 360)
		rotate = 0;
}

//--------------------------------------------------------------
void ofApp::draw(){
	 ofPushMatrix();
	 {
		 float w = ofGetWindowWidth();
		 float h = ofGetWindowHeight();
		 ofTranslate(w/2, h/2);
		 ofClear(color);
		 ofSetColor(0,0,0);
		 ofRotateZ(rotate);
		 ofDrawRectangle(-150,-150, 0, 300, 300);
	 }
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
	int r = ofRandom(255);
	int g = ofRandom(255);
	int b = ofRandom(255);
	color = ofColor(r, g, b);
}

//--------------------------------------------------------------
void ofApp::touchMoved(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchUp(int x, int y, int id){
//	color = ofColor(g_rotation, 255, 255);
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
