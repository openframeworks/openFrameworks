#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	 

	vagRounded.load("vag.ttf", 32);
	ofBackground(50,50,50);	
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	
	timeString = "time: " + ofGetTimestampString("%H:%M:%S") + "\nelapsed time: " + ofToString(ofGetElapsedTimeMillis());
		
	ofSetHexColor(0xffffff);
	vagRounded.drawString(timeString, 98,98);
	
	ofSetColor(255,122,220);
	vagRounded.drawString(timeString, 100,100);


	for(auto finger: touches){
		ofDrawCircle(finger.second, 50);
	}
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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::touchDown(int x, int y, int id){
	touches[id] = ofVec2f(x, y);
}

//--------------------------------------------------------------
void ofApp::touchMoved(int x, int y, int id){
	touches[id].set(x, y);
}

//--------------------------------------------------------------
void ofApp::touchUp(int x, int y, int id){
	touches.erase(id);
}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchCancelled(int x, int y, int id){
	
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
