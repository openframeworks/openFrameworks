#include "TriangleApp.h"

//--------------------------------------------------------------
TriangleApp :: TriangleApp () {
    cout << "creating TriangleApp" << endl;
}

//--------------------------------------------------------------
TriangleApp :: ~TriangleApp () {
    cout << "destroying TriangleApp" << endl;
}

//--------------------------------------------------------------
void TriangleApp::setup() {	
	ofBackground(127);
    
    int fontSize = 8;
    if (ofxiPhoneGetOFWindow()->isRetinaSupported())
        fontSize *= 2;
    
    font.loadFont("fonts/mono0755.ttf", fontSize);
}

//--------------------------------------------------------------
void TriangleApp::update(){

}

//--------------------------------------------------------------
void TriangleApp::draw() {
    int a = MIN(ofGetWidth(), ofGetHeight()) * 0.3;
    int b = sqrt( a * a + a * a );
    int x = ofGetWidth()  * 0.5;
    int y = ofGetHeight() * 0.5;
    int p = 0;
    
	ofSetColor(ofColor::red);
    ofTriangle(x, y - a, x + b, y + b, x - b, y + b);
    
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
void TriangleApp::exit() {
    //
}

//--------------------------------------------------------------
void TriangleApp::touchDown(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void TriangleApp::touchMoved(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void TriangleApp::touchUp(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void TriangleApp::touchDoubleTap(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void TriangleApp::lostFocus(){

}

//--------------------------------------------------------------
void TriangleApp::gotFocus(){

}

//--------------------------------------------------------------
void TriangleApp::gotMemoryWarning(){

}

//--------------------------------------------------------------
void TriangleApp::deviceOrientationChanged(int newOrientation){

}


//--------------------------------------------------------------
void TriangleApp::touchCancelled(ofTouchEventArgs& args){

}

