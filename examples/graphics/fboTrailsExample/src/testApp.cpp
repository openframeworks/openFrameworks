#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    ofBackground(0,0,0);
    
    ofFbo::Settings s;
	s.width				= 400;
	s.height			= 400;
	s.internalformat    = GL_RGBA;
    
	nonFloatingPointFbo.allocate(s);
    
    s.internalformat    = GL_RGBA32F_ARB;
    
    floatingPointFbo.allocate(s);
    
    nonFloatingPointFbo.begin();
    ofClear(255,255,255, 0);
    nonFloatingPointFbo.end();
    
    floatingPointFbo.begin();
    ofClear(255,255,255, 0);
    floatingPointFbo.end();
    
    
}

//--------------------------------------------------------------
void testApp::update(){

    ofEnableAlphaBlending();
    
    nonFloatingPointFbo.begin();
    
    ofFill();
    ofSetColor(255,255,255,2);
    ofRect(0,0,400,400);
    
    ofNoFill();
    ofSetColor(255,255,255);
    glPushMatrix();
        glTranslatef(200,200,0);
        glRotatef(ofGetElapsedTimef()*30, 1,0,0.5);
        ofBox(0,0, 100);
    glPopMatrix();
    
    nonFloatingPointFbo.end();
    
    floatingPointFbo.begin();
    
    ofFill();
    ofSetColor(255,255,255,2);
    ofRect(0,0,400,400);
    
    ofNoFill();
    ofSetColor(255,255,255);
    glPushMatrix();
    glTranslatef(200,200,0);
    glRotatef(ofGetElapsedTimef()*30, 1,0,0.5);
    ofBox(0,0, 100);
    glPopMatrix();
    
    floatingPointFbo.end();
    
}

//--------------------------------------------------------------
void testApp::draw(){

    
    ofSetColor(255,255,255);
    nonFloatingPointFbo.draw(0,0);
    floatingPointFbo.draw(450,0);
    
    ofDrawBitmapString("non floating point FBO", ofPoint(10,410));
    ofDrawBitmapString("floating point FBO", ofPoint(460,410));
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}