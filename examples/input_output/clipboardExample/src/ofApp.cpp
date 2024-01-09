#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	clipboardContents = "<empty>";
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground(0);
	ofSetColor(255);

	ofDrawBitmapString("openFrameworks can programatically interact with the clipboard!", 10, 14);
	ofDrawBitmapString("    Press 1 to send \"Hello World!\" to the clipboard.", 10, 28);
	ofDrawBitmapString("    Press 2 to see the clipboard contents: " + clipboardContents, 10, 42);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if(key == '1') {
		ofGetWindowPtr()->setClipboardString("Hello World!");
	} else if(key == '2') {
		clipboardContents = ofGetWindowPtr()->getClipboardString();
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

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
