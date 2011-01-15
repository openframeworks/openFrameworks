#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	
	
	// start camera, with radius of 100
}

//--------------------------------------------------------------
void testApp::update(){
}


//--------------------------------------------------------------
void testApp::draw(){

	cam.begin();
	
	// draw world axis
	float axisLength = 100;
	// draw world x axis
	ofSetColor(255, 0, 0);
	glPushMatrix();
	glTranslatef(axisLength/2, 0, 0);
	glScalef(axisLength, 2, 2);
	ofBox(0, 0, 0, 1);
	glPopMatrix();
	
	// draw world y axis
	ofSetColor(0, 255, 0);
	glPushMatrix();
	glTranslatef(0, axisLength/2, 0);
	glScalef(2, axisLength, 2);
	ofBox(0, 0, 0, 1);
	glPopMatrix();
	
	// draw world z axis
	ofSetColor(0, 0, 255);
	glPushMatrix();
	glTranslatef(0, 0, axisLength/2);
	glScalef(2, 2, axisLength);
	ofBox(0, 0, 0, 1);
	glPopMatrix();
	
	
	ofSetColor(255, 255, 0);
	ofBox(20);
	
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

