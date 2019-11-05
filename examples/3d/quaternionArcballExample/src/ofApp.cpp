/**
 *
 * OFDevCon Example Code Sprint
 * Quaternion Example for rotating a sphere as an arcball
 * Dragging the mouse up down left right to apply an intuitive rotation to an object.
 *
 * Created by James George on 2/23/2012
 */

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofNoFill();
	ofSetFrameRate(30);
	ofBackground(0);

	//this slows down the rotate a little bit
	dampen = .4;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

	//translate so that 0,0 is the center of the screen
	ofPushMatrix();
	ofTranslate(ofGetWidth()/2, ofGetHeight()/2, 40);

	auto axis = glm::axis(curRot);
	//apply the quaternion's rotation to the viewport and draw the sphere
	ofRotateDeg(ofRadToDeg(glm::angle(curRot)), axis.x, axis.y, axis.z);
	/// You can actually use the folling line instead, just showing this other option as example
	///	ofRotateRad(glm::angle(curRot), axis.x, axis.y, axis.z);
	
	
	ofDrawSphere(0, 0, 0, 200);

	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

	//every time the mouse is dragged, track the change
	//accumulate the changes inside of curRot through multiplication
	glm::vec2 mouse(x,y);
	glm::quat yRot = glm::angleAxis(ofDegToRad(x-lastMouse.x)*dampen, glm::vec3(0,1,0));
	glm::quat xRot = glm::angleAxis(ofDegToRad(y-lastMouse.y)*dampen, glm::vec3(-1,0,0));
	curRot = xRot * yRot * curRot;
    lastMouse = mouse;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    //store the last mouse point when it's first pressed to prevent popping
	lastMouse = glm::vec2(x,y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
