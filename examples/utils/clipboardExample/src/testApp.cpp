#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
    clipboardContents = "<empty>";
}

//--------------------------------------------------------------
void testApp::update() {
}

//--------------------------------------------------------------
void testApp::draw() {
	ofBackground(0);
	ofSetColor(255);

    ofDrawBitmapString("openFrameworks can programatically interact with the clipboard!", 10, 14);
	ofDrawBitmapString("    Press 1 to send \"Hello World!\" to the clipboard.", 10, 28);
    ofDrawBitmapString("    Press 2 to see the clipboard contents: " + clipboardContents, 10, 42);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    if(key == '1') {
        ofGetWindowPtr()->setClipboardString("Hello World!");
    } else if(key == '2') {
        clipboardContents = ofGetWindowPtr()->getClipboardString();
    }
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
