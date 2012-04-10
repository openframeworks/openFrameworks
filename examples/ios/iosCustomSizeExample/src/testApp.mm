#include "testApp.h"

//NOTE: in this example testApp ( this class ) is not used. 
//      look inside Apps/ to see the native iOS implementation

//--------------------------------------------------------------
void testApp::setup(){	
	// register touch events
	ofRegisterTouchEvents(this);
	
	// initialize the accelerometer
	ofxAccelerometer.setup();
	
	//iPhoneAlerts will be sent to this.
	ofxiPhoneAlerts.addListener(this);
	
	ofBackground(70);
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
    
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
void testApp::exit(){
    ofUnregisterTouchEvents(this);
    ofxiPhoneAlerts.removeListener(this);
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

