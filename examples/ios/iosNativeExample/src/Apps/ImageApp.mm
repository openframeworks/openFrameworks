#include "ImageApp.h"

ImageApp :: ImageApp () {
    cout << "creating ImageApp" << endl;
}

ImageApp :: ~ImageApp () {
    cout << "destroying ImageApp" << endl;
}

//--------------------------------------------------------------
void ImageApp::setup() {	
	ofRegisterTouchEvents(this);
	ofxAccelerometer.setup();
	ofxiPhoneAlerts.addListener(this);
	ofBackground(127);
    
    int fontSize = 8;
    if (ofxiPhoneGetOFWindow()->isRetinaSupported())
        fontSize *= 2;
    
    font.loadFont("fonts/mono0755.ttf", fontSize);
    
    image.loadImage("images/dive.jpg");
}

//--------------------------------------------------------------
void ImageApp::update(){

}

//--------------------------------------------------------------
void ImageApp::draw() {
    int x = (ofGetWidth()  - image.width)  * 0.5;
    int y = (ofGetHeight() - image.height) * 0.5;
    int p = 0;
    
	ofSetColor(ofColor::white);
    image.draw(x, y);
    
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
void ImageApp::exit() {
    ofUnregisterTouchEvents(this);
    ofxiPhoneAlerts.removeListener(this);
}

//--------------------------------------------------------------
void ImageApp::touchDown(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void ImageApp::touchMoved(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void ImageApp::touchUp(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void ImageApp::touchDoubleTap(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void ImageApp::lostFocus(){

}

//--------------------------------------------------------------
void ImageApp::gotFocus(){

}

//--------------------------------------------------------------
void ImageApp::gotMemoryWarning(){

}

//--------------------------------------------------------------
void ImageApp::deviceOrientationChanged(int newOrientation){

}


//--------------------------------------------------------------
void ImageApp::touchCancelled(ofTouchEventArgs& args){

}

