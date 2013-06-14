#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	font.loadFont("frabk.ttf",14);
	ofBackground(255,255,255);
	ofSetColor(0,0,0);
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

	font.drawString("hello world!!",20,20);
	//ofDrawBitmapString(ofToString(ofGetFrameRate()),20,30);
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::touchDown(int x, int y, int id){

}

//--------------------------------------------------------------
void testApp::touchMoved(int x, int y, int id){

}

//--------------------------------------------------------------
void testApp::touchUp(int x, int y, int id){

}

//--------------------------------------------------------------
void testApp::touchDoubleTap(int x, int y, int id){

}

//--------------------------------------------------------------
void testApp::touchCancelled(int x, int y, int id){

}

//--------------------------------------------------------------
void testApp::swipe(ofxAndroidSwipeDir swipeDir, int id){

}

//--------------------------------------------------------------
void testApp::pause(){

}

//--------------------------------------------------------------
void testApp::stop(){

}

//--------------------------------------------------------------
void testApp::resume(){

}

//--------------------------------------------------------------
void testApp::reloadTextures(){

}

//--------------------------------------------------------------
bool testApp::backPressed(){
	return false;
}

//--------------------------------------------------------------
void testApp::okPressed(){

}

//--------------------------------------------------------------
void testApp::cancelPressed(){

}
