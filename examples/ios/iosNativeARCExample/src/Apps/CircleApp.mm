#include "CircleApp.h"

//--------------------------------------------------------------
CircleApp :: CircleApp () {
    cout << "creating CircleApp" << endl;
        //add this to listen to orientation events etc.
    ofxiOSAlerts.addListener(this);
}

//--------------------------------------------------------------
CircleApp :: ~CircleApp () {
    cout << "destroying CircleApp" << endl;
    //add this to listen to orientation events etc.
    ofxiOSAlerts.removeListener(this);
}

//--------------------------------------------------------------
void CircleApp::setup() {
	ofBackground(127);
    
    int fontSize = 8;
    if (ofxiOSGetOFWindow()-> isRetinaSupportedOnDevice())
        fontSize *= 2;
    
    font.load("fonts/frabk.ttf", fontSize);
    
}

//--------------------------------------------------------------
void CircleApp::update(){

}

//--------------------------------------------------------------
void CircleApp::draw() {
    int x = ofGetWidth()  * 0.5;
    int y = ofGetHeight() * 0.5;
    int r = MIN(ofGetWidth(), ofGetHeight()) * 0.3;
    int p = 0;
    
	ofSetColor(ofColor::red);
    ofDrawCircle(x, y, r);
    
    x = ofGetWidth()  * 0.2;
    y = ofGetHeight() * 0.11;
    p = ofGetHeight() * 0.035;
    
    ofSetColor(ofColor::white);
    font.drawString("frame num      = " + ofToString(ofGetFrameNum() ),    x, y+=p);
    font.drawString("frame rate     = " + ofToString(ofGetFrameRate() ),   x, y+=p);
    font.drawString("screen width   = " + ofToString(ofGetWidth() ),       x, y+=p);
    font.drawString("screen height  = " + ofToString(ofGetHeight() ),      x, y+=p);
}

//--------------------------------------------------------------
void CircleApp::exit() {
    //
}

//--------------------------------------------------------------
void CircleApp::touchDown(ofTouchEventArgs &touch){
    int orient = ofGetOrientation();
    if(orient<4){
        orient++;
    }else{
        orient = 1;
    }
    ofSetOrientation((ofOrientation)orient);
}

//--------------------------------------------------------------
void CircleApp::touchMoved(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void CircleApp::touchUp(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void CircleApp::touchDoubleTap(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void CircleApp::lostFocus(){

}

//--------------------------------------------------------------
void CircleApp::gotFocus(){

}

//--------------------------------------------------------------
void CircleApp::gotMemoryWarning(){

}

//--------------------------------------------------------------
void CircleApp::deviceOrientationChanged(int newOrientation){
    if (UIDeviceOrientationIsLandscape([UIDevice currentDevice].orientation)){
         // code for landscape orientation
         cout<<"CircleApp::deviceOrientationChanged to landscape"<<endl;
    }else{
        cout<<"CircleApp::deviceOrientationChanged portrait"<<endl;
    }
    
}


//--------------------------------------------------------------
void CircleApp::touchCancelled(ofTouchEventArgs& args){

}

