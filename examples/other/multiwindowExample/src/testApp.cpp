#include "testApp.h"

int i = 0;

//--------------------------------------------------------------
void testApp::setup(){
	ofSetBackgroundColor(255, 100, 100);
	ofGetMainWindow()->setWindowShape(600, 600);
	ofCreateWindow(500, 500, 200, 200)->addListener(new windowRenderer);
	/*
	ofCreateWindow(300, 400, 100, 100);
	ofCreateWindow(600, 600, 100, 100);
	ofGetMainWindow()->setWindowPosition(200, 200);
	 * */
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(50, 0, 0);
	ofRect(i%ofGetWidth(), 100, 10, 10);
	i++;

	ofSetColor(255, 0, 0);
	ofRect(ofGetMouseX(), ofGetMouseY(), 50, 50);
	
	ofSetColor(0);
	ofDrawBitmapString(ofToString(ofGetFrameRate()), 20, 20);
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
void testApp::scrolled(float dx, float dy)
{
	ofLogNotice() << dx << "- " << dy;
}
