#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	player_.load("hands.m4v");
	player_.play();
}

//--------------------------------------------------------------
void ofApp::update(){
	ofBackground(255,255,255);
	player_.update();
}

//--------------------------------------------------------------
void ofApp::draw(){	
	player_.draw(0,0);
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


