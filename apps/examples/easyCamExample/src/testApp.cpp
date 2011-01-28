#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	
	// this uses depth information for occlusion
	// rather than always drawing things on top of each other
	glEnable(GL_DEPTH_TEST);
	
	// this sets the camera's distance from the object
	cam.setDistance(100);
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

	cam.begin();		
		ofRotateX(ofRadToDeg(.5));
		ofRotateY(ofRadToDeg(-.5));

		ofBackground(0);

		ofSetColor(255,0,0);
		ofFill();
		ofBox(30);
		ofNoFill();
		ofSetColor(0);
		ofBox(30);

		ofPushMatrix();
			ofTranslate(0,0,20);
			ofSetColor(0,0,255);
			ofFill();
			ofBox(5);
			ofNoFill();
			ofSetColor(0);
			ofBox(5);
		ofPopMatrix();
	cam.end();

	ofSetColor(255);
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

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
