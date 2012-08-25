#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	
	// register touch events
	ofRegisterTouchEvents(this);
	
	// initialize the accelerometer
	ofxAccelerometer.setup();
	
	//iPhoneAlerts will be sent to this.
	ofxiPhoneAlerts.addListener(this);
	
	//If you want a landscape oreintation 
	//iPhoneSetOrientation(OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT);
	
	ofBackground(127,127,127);
    
    img.loadImage("photo.jpg");
    
    bToggleCenterMode = false;
    
    // the masking rectangle
    rec.set(ofGetMouseX(), ofGetMouseY(), 100, 100);
    

}

//--------------------------------------------------------------
void testApp::update(){
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    ofEnableAlphaBlending();
    
    
    // draw it normally
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetColor(255, 20);
    img.draw((ofGetWidth()-img.getWidth())/2, (ofGetHeight()-img.getHeight())/2);
    
    
    // toggle the rect mode
    if(bToggleCenterMode) ofSetRectMode(OF_RECTMODE_CENTER);
    else                  ofSetRectMode(OF_RECTMODE_CORNER);
    
    
    // center the image
    ofPoint center((ofGetWidth()-img.getWidth())/2, (ofGetHeight()-img.getHeight())/2);
    if(bToggleCenterMode) center.set(ofGetWidth()/2, ofGetHeight()/2);
    
    
    // scissor out the image with our rect   
    ofBeginScissor(rec);
    ofSetColor(255);
    img.draw(center);
    ofEndScissor();
    
    // the rect
    ofNoFill();
    ofSetColor(0, 255, 255);
    ofRect(rec);
    
}

//--------------------------------------------------------------
void testApp::exit(){

}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs &touch){
    
   
    if(touch.numTouches == 1) {
        // set the rec pos
        rec.x = touch.x;
        rec.y = touch.y;
        firstPt.set(touch.x, touch.y);
        
    }
    if(touch.numTouches == 2) {
        ofVec2f vec = ofVec2f(touch.x, touch.y) - firstPt;
        float   len = vec.length();
        rec.width   = len;
        rec.height  = len;
    }
    
}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs &touch){
    bToggleCenterMode = !bToggleCenterMode;
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


//--------------------------------------------------------------
void testApp::touchCancelled(ofTouchEventArgs& args){

}

