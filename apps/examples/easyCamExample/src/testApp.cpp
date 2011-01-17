#include "testApp.h"

ofNode target;
ofRectangle rect;
bool oldMousePress = false;
ofVec3f mousePrev;

//--------------------------------------------------------------
void testApp::setup(){
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	
	ofSetVerticalSync(true);
}

//--------------------------------------------------------------
void testApp::update(){
	rect.width = ofGetWidth();
	rect.height = ofGetHeight();
	
	
//	oldMousePress = ofGetMousePressed(0);
//	mousePrev = mousePos;
	
}


//--------------------------------------------------------------
void testApp::draw(){

	cam.begin();
	
	ofDrawAxis(200);	
	ofSetColor(255, 255, 0);
	ofBox(20);
//	target.draw();
	
//	cam.getTarget().draw();
	
	
	cam.end();
	
	ofDisableLighting();
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("This text is not affected by camera because it is after cam.end()", 10, 30);
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

