#include "ofApp.h"

bool locked = false;

//--------------------------------------------------------------
void ofApp::setup(){
	mainAppsCount = 0;
	TO.start();
}

//--------------------------------------------------------------
void ofApp::update(){
	ofBackground(0,0,0);   // black because threads are EVIL ;)
	mainAppsCount++;
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetHexColor(0xffffff);
	TO.draw();

    string str = "I am a the main opengl thread.\nmy current count is: ";
	str += ofToString(mainAppsCount);
    ofDrawBitmapString(str, 350, 56);


    ofSetHexColor(0xff0033);

    ofDrawBitmapString("press 's' to stop the thread\npress 'a' to start the thread\npress '!' to test the exception handler (will log an error and stop the thread).", 50, 160);
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){

    if (key == 'a'){
        TO.start();
    } else if (key == 's'){
        TO.stop();
    } else if (key == '!'){
        TO.throwErrorTest();
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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}