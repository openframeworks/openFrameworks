#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	
	// this uses depth information for occlusion
	// rather than always drawing things on top of each other
	ofEnableDepthTest();
	
	// this sets the camera's distance from the object
	cam.setDistance(100);
	
	ofSetCircleResolution(64);
	bShowHelp = true;
}

//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	cam.begin();		
	ofRotateX(ofRadToDeg(.5));
	ofRotateY(ofRadToDeg(-.5));
	
	ofBackground(0);
	
	ofSetColor(255,0,0);
	ofFill();
	ofDrawBox(30);
	ofNoFill();
	ofSetColor(0);
	ofDrawBox(30);
	
	ofPushMatrix();
	ofTranslate(0,0,20);
	ofSetColor(0,0,255);
	ofFill();
	ofDrawBox(5);
	ofNoFill();
	ofSetColor(0);
	ofDrawBox(5);
	ofPopMatrix();
	cam.end();
	drawInteractionArea();
	ofSetColor(255);
	string msg = string("Using mouse inputs to navigate (press 'c' to toggle): ") + (cam.getMouseInputEnabled() ? "YES" : "NO");
	msg += string("\nShowing help (press 'h' to toggle): ")+ (bShowHelp ? "YES" : "NO");
	if (bShowHelp) {
		msg += "\n\nLEFT MOUSE BUTTON DRAG:\nStart dragging INSIDE the yellow circle -> camera XY rotation .\nStart dragging OUTSIDE the yellow circle -> camera Z rotation (roll).\n\n";
		msg += "LEFT MOUSE BUTTON DRAG + TRANSLATION KEY (" + ofToString(cam.getTranslationKey()) + ") PRESSED\n";
		msg += "OR MIDDLE MOUSE BUTTON (if available):\n";
		msg += "move over XY axes (truck and boom).\n\n";
		msg += "RIGHT MOUSE BUTTON:\n";
		msg += "move over Z axis (dolly)";
	}
	msg += "\n\nfps: " + ofToString(ofGetFrameRate(), 2);
	ofDrawBitmapStringHighlight(msg, 10, 20);
}
//--------------------------------------------------------------
void ofApp::drawInteractionArea(){
	ofRectangle vp = ofGetCurrentViewport();
	float r = MIN(vp.width, vp.height) * 0.5f;
	float x = vp.width * 0.5f;
	float y = vp.height * 0.5f;
	
	ofPushStyle();
	ofSetLineWidth(3);
	ofSetColor(255, 255, 0);
	ofNoFill();
	glDepthMask(false);
	ofDrawCircle(x, y, r);
	glDepthMask(true);
	ofPopStyle();
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key) {
		case 'C':
		case 'c':
			if(cam.getMouseInputEnabled()) cam.disableMouseInput();
			else cam.enableMouseInput();
			break;
			
		case 'F':
		case 'f':
			ofToggleFullscreen();
			break;
		case 'H':
		case 'h':
			bShowHelp ^=true;
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
	
}
