#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	 

	number = 0;
}

//--------------------------------------------------------------
void ofApp::update(){

	// maybe some more explanation about 
	// things that will not happen in headless mode?
	// keystrokes? events? sound? rendering?
	
	// everything that does not run on screen runs as expected
	number++;
	ofLogNotice() << number;

}

//--------------------------------------------------------------
void ofApp::draw(){
	
	// these circles will never be drawn
	for (int i = 0; i < number; i++) {
		ofDrawCircle(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()), 15);
	}
	
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){ 

	// Currently keystrokes have no effect.
	// The number will not be reset to 0.
	if (key == 'r') {
		number = 0;
	}
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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
