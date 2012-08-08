#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(255,255,255);

	fingerMovie.loadMovie("sample_mpeg4.mp4");
	fingerMovie.play();
}

//--------------------------------------------------------------
void testApp::update(){
    fingerMovie.idleMovie();
}

float vol = 0;

//--------------------------------------------------------------
void testApp::draw(){

	ofSetHexColor(0xFFFFFF);
    fingerMovie.draw(20,20);

    ofSetHexColor(0x000000);
	ofDrawBitmapString("volume: " + ofToString(vol),20,440 );
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	vol = ofClamp((float)x/ofGetWidth(),0,1);
	fingerMovie.setVolume(vol);
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
