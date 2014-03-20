#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	
	
    // initialize the accelerometer
	ofxAccelerometer.setup();
	
	ofBackground(70);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetLineWidth(2);
    
    ofPoint p1;
    ofPoint p2;
    
    //-- draw width arrow.
    
    p1.set(15, ofGetHeight()-10);
    p2.set(ofGetWidth()-15, ofGetHeight()-10);
    
	ofSetColor(ofColor::magenta);
    ofPushMatrix();
    {
        ofTranslate(p1.x, p1.y);
        ofRotate(45);
        ofLine(0, 0, 0, -10);
        ofRotate(90);
        ofLine(0, 0, 0, -10);
    }
    ofPopMatrix();
    ofPushMatrix();
    {
        ofTranslate(p2.x, p2.y);
        ofRotate(-45);
        ofLine(0, 0, 0, -10);
        ofRotate(-90);
        ofLine(0, 0, 0, -10);
    }
    ofPopMatrix();
    ofLine( p1.x, p1.y, p2.x, p2.y);
    ofDrawBitmapString(ofToString(ofGetWidth()), ofGetWidth()*0.5-10, ofGetHeight()-20);
    
    //-- draw height arrow.
    
    p1.set(ofGetWidth()-10, 15);
    p2.set(ofGetWidth()-10, ofGetHeight()-15);
    
    ofSetColor(ofColor::green);
    ofPushMatrix();
    {
        ofTranslate(p1.x, p1.y);
        ofRotate(-135);
        ofLine(0, 0, 0, -10);
        ofRotate(-90);
        ofLine(0, 0, 0, -10);
    }
    ofPopMatrix();
    ofPushMatrix();
    {
        ofTranslate(p2.x, p2.y);
        ofRotate(-45);
        ofLine(0, 0, 0, -10);
        ofRotate(-270);
        ofLine(0, 0, 0, -10);
    }
    ofPopMatrix();
    ofLine( p1.x, p1.y, p2.x, p2.y);
    ofDrawBitmapString(ofToString(ofGetHeight()), ofGetWidth()-40, ofGetHeight()*0.5-5);
    
    ofSetColor(ofColor::white);
    ofDrawBitmapString("touch x = " + ofToString(mouseX), ofGetWidth()*0.1, ofGetHeight()*0.5-46);
    ofDrawBitmapString("touch y = " + ofToString(mouseY), ofGetWidth()*0.1, ofGetHeight()*0.5-30);
}

//--------------------------------------------------------------
void ofApp::exit(){
    //
}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchCancelled(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void ofApp::lostFocus(){

}

//--------------------------------------------------------------
void ofApp::gotFocus(){

}

//--------------------------------------------------------------
void ofApp::gotMemoryWarning(){

}

//--------------------------------------------------------------
void ofApp::deviceOrientationChanged(int newOrientation){

}

