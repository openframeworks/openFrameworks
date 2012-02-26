#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
    
    img.loadImage("photo.jpg");
    
    bToggleCenterMode = false;
}

//--------------------------------------------------------------
void testApp::update() {
    
}

//--------------------------------------------------------------
void testApp::draw() {
    
    ofEnableAlphaBlending();
    
    
    // draw it normally
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetColor(255, 20);
    img.draw((ofGetWidth()-img.getWidth())/2, (ofGetHeight()-img.getHeight())/2);
    
    
    // toggle the rect mode
    if(bToggleCenterMode) ofSetRectMode(OF_RECTMODE_CENTER);
    else                  ofSetRectMode(OF_RECTMODE_CORNER);

    
    // the masking rectangle
    ofRectangle rec(ofGetMouseX(), ofGetMouseY(), 100, 100);
    
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
void testApp::keyPressed(int key) {
    bToggleCenterMode = !bToggleCenterMode;
    
}


//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
}

