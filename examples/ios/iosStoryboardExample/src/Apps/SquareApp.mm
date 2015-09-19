#include "SquareApp.h"

//--------------------------------------------------------------
SquareApp :: SquareApp () {
    cout << "creating SquareApp" << endl;
}

//--------------------------------------------------------------
SquareApp :: ~SquareApp () {
    cout << "destroying SquareApp" << endl;
}

//--------------------------------------------------------------
void SquareApp::setup() {	
	ofBackground(127);
    ofSetOrientation(OF_ORIENTATION_DEFAULT);
    
    int fontSize = 8;
    if (ofxiOSGetOFWindow()->isRetinaSupportedOnDevice())
        fontSize *= 2;

    font.load("fonts/mono0755.ttf", fontSize);
}

//--------------------------------------------------------------
void SquareApp::update(){

}

//--------------------------------------------------------------
void SquareApp::draw() {
    ofEnableAlphaBlending();
    
    int w = MIN(ofGetWidth(), ofGetHeight()) * 0.6;
    int h = w;
    int x = (ofGetWidth() - w)  * 0.5;
    int y = (ofGetHeight() - h) * 0.5;
    int p = 0;
    
	ofSetColor(ofColor::red);
    ofDrawRectangle(x, y, w, h);
    
    x = ofGetWidth()  * 0.2;
    y = ofGetHeight() * 0.11;
    p = ofGetHeight() * 0.035;
    
    ofSetColor(ofColor::white);
    font.drawString("frame num      = " + ofToString( ofGetFrameNum() ),    x, y+=p);
    font.drawString("frame rate     = " + ofToString( ofGetFrameRate() ),   x, y+=p);
    font.drawString("screen width   = " + ofToString( ofGetWidth() ),       x, y+=p);
    font.drawString("screen height  = " + ofToString( ofGetHeight() ),      x, y+=p);
}

//--------------------------------------------------------------
void SquareApp::exit() {
    //
}

//--------------------------------------------------------------
void SquareApp::touchDown(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void SquareApp::touchMoved(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void SquareApp::touchUp(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void SquareApp::touchDoubleTap(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void SquareApp::lostFocus(){

}

//--------------------------------------------------------------
void SquareApp::gotFocus(){

}

//--------------------------------------------------------------
void SquareApp::gotMemoryWarning(){

}

//--------------------------------------------------------------
void SquareApp::deviceOrientationChanged(int newOrientation){

}


//--------------------------------------------------------------
void SquareApp::touchCancelled(ofTouchEventArgs& args){

}

