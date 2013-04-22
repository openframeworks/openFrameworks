/**
 *
 * OFDevCon Example Code Sprint
 * Quaternion Example for rotating a sphere as an arcball
 * Dragging the mouse up down left right to apply an intuitive rotation to an object.
 *
 * Created by James George on 2/23/2012
 */

#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofNoFill();
	ofSetFrameRate(30);
	ofBackground(0);
	
	//this slows down the rotate a little bit
	dampen = .4;
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	
	//translate so that 0,0 is the center of the screen
    glPushMatrix();  
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2, 40);  
	//Extract the rotation from the current rotation
    ofVec3f axis;  
    float angle;  
    curRot.getRotate(angle, axis);  
	
	//apply the quaternion's rotation to the viewport and draw the sphere
	ofRotate(angle, axis.x, axis.y, axis.z);  
	ofSphere(0, 0, 0, 200);
	
	ofPopMatrix();  
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
	
	//every time the mouse is dragged, track the change
	//accumulate the changes inside of curRot through multiplication
    ofVec2f mouse(x,y);  
    ofQuaternion yRot((x-lastMouse.x)*dampen, ofVec3f(0,1,0));  
    ofQuaternion xRot((y-lastMouse.y)*dampen, ofVec3f(-1,0,0));  
    curRot *= yRot*xRot;  
    lastMouse = mouse;  
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    //store the last mouse point when it's first pressed to prevent popping
	lastMouse = ofVec2f(x,y);
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