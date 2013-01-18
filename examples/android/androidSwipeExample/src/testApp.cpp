#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofRegisterTouchEvents((ofxAndroidApp*)this);
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
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

void testApp::touchDown(int x, int y, int id){
	ofLogNotice() << "touch down " << x << "," << y << ":" << id;

}

void testApp::touchMoved(int x, int y, int id){
	ofLogNotice() << "touch moved " << x << "," << y << ":" << id;

}

void testApp::touchUp(int x, int y, int id){
	ofLogNotice() << "touch up " << x << "," << y << ":" << id;

}

void testApp::touchDoubleTap(int x, int y, int id){
	ofLogNotice() << "touch double " << x << "," << y << ":" << id;

}

void testApp::touchCancelled(int x, int y, int id){
	ofLogNotice() << "touch cacelled " << x << "," << y << ":" << id;

}

void testApp::swipe(ofxAndroidSwipeDir swipeDir, int id){
	ofLogNotice() << "swipes " << swipeDir << id;
}
