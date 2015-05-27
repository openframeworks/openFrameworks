#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(255,255,255);

	fingerMovie.loadMovie("sample_mpeg4.mp4");
	fingerMovie.play();
}

//--------------------------------------------------------------
void ofApp::update(){
    fingerMovie.idleMovie();
}

float vol = 0;

//--------------------------------------------------------------
void ofApp::draw(){

	ofSetHexColor(0xFFFFFF);
    fingerMovie.draw(20,20);

    ofSetHexColor(0x000000);
	ofDrawBitmapString("volume: " + ofToString(vol),20,440 );
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
	vol = ofClamp((float)x/ofGetWidth(),0,1);
	fingerMovie.setVolume(vol);
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
