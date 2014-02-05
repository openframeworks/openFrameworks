#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	player.loadMovie("fingers.mov");
	player.play();
    player.setLoopState( OF_LOOP_NORMAL );
#ifdef TARGET_32_BIT
    cout << "Found the 32 bit version " << endl;
#endif
    
#ifdef TARGET_64_BIT
    cout << "FOund the 64 bit version" << endl;
#endif
    
#ifdef OF_VIDEO_PLAYER_AVFOUNDATION
    cout << "Using the AVFoundationVideo Player " << endl;
#endif
    
#ifdef OF_VIDEO_PLAYER_QTKIT
    cout << "USing the QTKit video player" << endl;
#endif
}

//--------------------------------------------------------------
void ofApp::update(){
	player.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	player.draw(0,0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
