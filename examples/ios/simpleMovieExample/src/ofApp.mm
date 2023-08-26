#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	player_.load("hands.m4v");
	player_.play();
}

//--------------------------------------------------------------
void ofApp::update(){
	player_.update();
	
	if (ofGetElapsedTimef() > 10) {
		player_.close();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0,0,0);
	if (player_.isLoaded()) {
		player_.draw(0,0);

	} else {
		ofDrawBitmapString("Player was unloaded sucessfully after 10 seconds", 20,20);
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


