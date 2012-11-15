#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofxAccelerometer.setup();
	iPhoneSetOrientation(OFXIPHONE_ORIENTATION_PORTRAIT);
	ofBackground(127,127,127);
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
    ofPushStyle();
    ofFill();
    ofSetColor(ofColor::red);
    ofRect(10, 10, 100, 100);
    ofPopStyle();
    
    ofPushStyle();
    ofNoFill();
    ofSetColor(ofColor::green);
    ofRect(10, 10, 100, 100);
    ofPopStyle();
    
    ofPushStyle();
    ofFill();
    ofSetColor(ofColor::red);
    ofTriangle(180, 10, 130, 110, 230, 110);
    ofPopStyle();
    
    ofPushStyle();
    ofNoFill();
    ofSetColor(ofColor::green);
    ofTriangle(180, 10, 130, 110, 230, 110);
    ofPopStyle();
    
    ofPushStyle();
    ofFill();
    ofSetColor(ofColor::red);
    ofCircle(60, 170, 50);
    ofPopStyle();
    
    ofPushStyle();
    ofNoFill();
    ofSetColor(ofColor::green);
    ofCircle(60, 170, 50);
    ofPopStyle();
    
    ofPushStyle();
    ofFill();
    ofSetColor(ofColor::red);
    ofEllipse(180, 170, 100, 50);
    ofPopStyle();

    ofPushStyle();
    ofNoFill();
    ofSetColor(ofColor::green);
    ofEllipse(180, 170, 100, 50);
    ofPopStyle();
    
    ofPushStyle();
    ofEnableSmoothing();
    ofFill();
    ofSetColor(ofColor::red);
    ofLine(260, 120, 300, 220);
    ofPopStyle();
    
}

//--------------------------------------------------------------
void testApp::exit(){

}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void testApp::touchCancelled(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void testApp::lostFocus(){

}

//--------------------------------------------------------------
void testApp::gotFocus(){

}

//--------------------------------------------------------------
void testApp::gotMemoryWarning(){

}

//--------------------------------------------------------------
void testApp::deviceOrientationChanged(int newOrientation){

}

