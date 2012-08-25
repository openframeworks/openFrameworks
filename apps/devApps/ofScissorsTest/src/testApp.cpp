#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
    
    img.loadImage("photo.jpg");
    bToggleCenterMode = false;
    bToggleFBO        = false;
    bSavePDF          = false;
    
    fbo.allocate(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void testApp::drawScissor() {
    
    ofEnableAlphaBlending();
    
    // draw it normally
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetColor(255, 20);
    //img.draw((ofGetWidth()-img.getWidth())/2, (ofGetHeight()-img.getHeight())/2);
    
    
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
    
    ofFill();
    ofSetColor(255, 0, 0);
    ofRect(center.x, center.y, 200, 200);
    
    ofEndScissor();
    
    // the rect
    ofNoFill();
    ofSetColor(0, 255, 255);
    ofRect(rec);
}

//--------------------------------------------------------------
void testApp::update() {
    
    if(bToggleFBO) {
        fbo.begin();
        ofClear(0, 0, 0);
        drawScissor();
        fbo.end();
    }
}

//--------------------------------------------------------------
void testApp::draw() {
    
    if(bToggleFBO) {
        ofSetColor(255);
        ofSetRectMode(OF_RECTMODE_CORNER);
        fbo.draw(0, 0);
    }
    else {
        drawScissor();
    }
    
    // save out a pdf
    if(bSavePDF) {
        ofBeginSaveScreenAsPDF("ofScissorTest.pdf");
        drawScissor();
        ofEndSaveScreenAsPDF();
        bSavePDF = false;
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
    if(key == 'c') bToggleCenterMode = !bToggleCenterMode;
    if(key == 'f') bToggleFBO = !bToggleFBO;
    if(key == 's') bSavePDF = !bSavePDF;
    
}


//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
}

