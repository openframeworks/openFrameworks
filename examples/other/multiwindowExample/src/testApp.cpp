#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	ofSetBackgroundColor(255, 100, 100);
	ofGetMainWindow()->setWindowShape(600, 600);
	ofWindow* secondaryWindow = ofCreateWindow(650, 0, ofRandom(400, 800), ofRandom(100, 800));
	secondaryWindow->addListener(new windowRenderer);

	textPos.set(200, 300);
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
	
	ofFill();
	ofSetColor(160);
	ofRect(ofGetFrameNum()%ofGetWidth(), 0, 2, ofGetHeight());

	ofSetColor(0, 0, 0);
	ofSetRectMode(OF_RECTMODE_CENTER);
	ofRect(ofGetMouseX(), ofGetMouseY(), 20, 20);
	
	ofSetColor(0);
	ofDrawBitmapString("THIS IS THE FIRST WINDOW "+ofToString(ofGetFrameRate()), 30, 30);

	ofSetRectMode(OF_RECTMODE_CORNER);
	ofDrawBitmapStringHighlight("SCROLL TO MOVE ME", textPos);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	ofGetMainWindow()->toggleFullscreen();
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
	textPos.x += dx;
	textPos.y += dy;
}
