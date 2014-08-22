#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofRegisterTouchEvents((ofxAndroidApp*)this);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){ 
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

void ofApp::touchDown(int x, int y, int id){
	ofLogNotice() << "touch down " << x << "," << y << ":" << id;

}

void ofApp::touchMoved(int x, int y, int id){
	ofLogNotice() << "touch moved " << x << "," << y << ":" << id;

}

void ofApp::touchUp(int x, int y, int id){
	ofLogNotice() << "touch up " << x << "," << y << ":" << id;

}

void ofApp::touchDoubleTap(int x, int y, int id){
	ofLogNotice() << "touch double " << x << "," << y << ":" << id;

}

void ofApp::touchCancelled(int x, int y, int id){
	ofLogNotice() << "touch cacelled " << x << "," << y << ":" << id;

}

void ofApp::swipe(ofxAndroidSwipeDir swipeDir, int id){
	ofLogNotice() << "swipes " << swipeDir << id;
}
