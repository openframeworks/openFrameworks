/**
 *
 * OFDevCon Example Code Sprint
 * Quaternion Example for rotating a sphere as an arcball
 * Dragging the mouse up down left right to apply an intuitive rotation to an object.
 *
 * Created by James George on 2/23/2012
 * Modified by Nick Hardeman on 9/20/2022
 */

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(30);

	ofSetCircleResolution(120);

	// draw a gradient in the background
	ofBackgroundGradient(ofColor(10), ofColor(50));

	//this slows down the rotate a little bit
	dampen = .4;
}

//--------------------------------------------------------------
void ofApp::update(){
	float deltaTime = ofClamp( ofGetLastFrameTime(), 1.f/5000.f, 1.f/5.f);
	// slow down the xspeed and yspeed with division
	xspeed /= 1.f + deltaTime;
	yspeed /= 1.f + deltaTime;

	glm::quat yRot = glm::angleAxis(ofDegToRad(xspeed), glm::vec3(0,1,0));
	glm::quat xRot = glm::angleAxis(ofDegToRad(yspeed), glm::vec3(-1,0,0));
	//accumulate the changes inside of curRot through multiplication
	curRot = xRot * yRot * curRot;
}

//--------------------------------------------------------------
void ofApp::draw(){
	// draw a gradient in the background
	ofBackgroundGradient(ofColor(10), ofColor(50));


	glm::vec3 center(ofGetWidth()/2, ofGetHeight()/2, 40);

	ofEnableDepthTest();
	//translate so that 0,0 is the center of the screen
	ofPushMatrix();
	ofTranslate(center);

	float size = 150;
	ofPushMatrix();

	ofMultMatrix( glm::mat4(curRot) );

	ofSetColor( 255 );
	ofDrawBox(0, 0, 0, size, size, size );

	float newSize = size * 1.5f;

	ofNoFill();
	ofSetColor( 150 );
	ofDrawCircle(0, 0, newSize);
	ofRotateXDeg(90);
	ofDrawCircle(0, 0, newSize);
	ofRotateYDeg(90);
	ofDrawCircle(0, 0, newSize);

	ofFill();

	float redValueFromXspeed = ofMap( fabs(xspeed), 1, 15, 100, 255, true );
	float greenValueFromYspeed = ofMap( fabs(yspeed), 1, 15, 100, 255, true );

	ofSetColor( redValueFromXspeed, greenValueFromYspeed, MIN(redValueFromXspeed, greenValueFromYspeed) );
	ofDrawSphere(0, -newSize, 0, 10.0 );
	ofDrawSphere(0, newSize, 0, 10.0 );
	ofDrawSphere(newSize, 0, 0, 10.0 );
	ofDrawSphere(-newSize, 0, 0, 10.0 );
	ofDrawSphere(0.0, 0, newSize, 10.0 );
	ofDrawSphere(0, 0, -newSize, 10.0 );

	ofPopMatrix();

	ofPopMatrix();

	newSize = size;

	// below are three different ways to derive rotation and position for drawing cones along the x,y and z axes

	// transform the position along the x axis by the quaternion
	glm::vec3 xPosition = curRot * glm::vec3(newSize, 0.0, 0.0);
	// add the center of the screen
	xPosition += center;
	ofFill();
	ofSetColor( ofColor::red );
	ofPushMatrix();
	ofTranslate(xPosition);
	ofMultMatrix( glm::mat4(curRot) );
	ofRotateZDeg(90);
	ofDrawCone(15, 45);
	ofPopMatrix();

	glm::mat4 ymat(1.0);
	ymat = glm::translate(ymat, curRot*glm::vec3(0,newSize,0.0)+center);
	// spin the cone around the x axis so that it's pointing in the opposite direction than it normally draws
	glm::quat extraXRot = glm::angleAxis(ofDegToRad(180), glm::vec3(1,0,0));
	ymat = ymat * glm::mat4_cast(curRot*extraXRot);
	ofPushMatrix();
	ofSetColor( ofColor::green );
	ofMultMatrix(ymat);
	ofDrawCone(15, 45);
	ofPopMatrix();

	ofNode zNode;
	zNode.setPosition( curRot * glm::vec3(0,0,newSize)+center);
	zNode.setOrientation( curRot );
	zNode.tiltDeg(-90);
	ofSetColor( ofColor::blue );
	zNode.transformGL();
	ofDrawCone(15, 45);
	zNode.restoreTransformGL();



	// draw some arrows on the screen showing the forces applied by the mouse
	ofPushMatrix();
	float arrowSize = 100.0;

	ofTranslate(arrowSize + 50, arrowSize + 50 );
	ofNoFill();
	ofSetColor( 70 );
	ofDrawCircle(0, 0, arrowSize);
	ofDrawCircle(0, 0, 10);
	ofFill();
	ofSetColor( redValueFromXspeed, 100, 100 );
	ofDrawArrow(glm::vec3(0,0,0), glm::vec3(arrowSize*xspeed/15.f, 0, 0), 15*fabs(xspeed)/15.f );

	ofSetColor( 100, greenValueFromYspeed, 100 );
	ofDrawArrow(glm::vec3(0,0,0), glm::vec3(0, arrowSize*yspeed/15.f, 0), 15*fabs(yspeed)/15.f );
	ofPopMatrix();

	ofSetColor( 230 );
	ofDrawBitmapString("Drag mouse to apply rotations", 20, 20);
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
	glm::vec2 mouse(x,y);
	//every time the mouse is dragged, track the change
	// accumulate the changes in x inside of xspeed
	// ofLerp eases the value of xspeed gradually
	xspeed = ofLerp(xspeed, (x-lastMouse.x) * dampen, 0.1);
	//accumulate the changes in y inside of yspeed
	yspeed = ofLerp(yspeed, (y-lastMouse.y) * dampen, 0.1);

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
