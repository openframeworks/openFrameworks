/**
 *
 * OFDevCon Example Code Sprint
 * Trigonometry example that depicts the relationship between a circle and the sine and cosine.
 *
 * Created by Roy Macdonald on 2/24/2012
 */

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	ofSetFrameRate(60);
	ofBackground(50);
		
	//initial values
	angle=0;
	cosine=0;
	sine=0;
	tangent=0;
	rotationSpeed=0.01;
	radius = 180;
	center = {ofGetWidth()*0.3f, ofGetHeight()*0.6f, 0 };
	
	ofSetCircleResolution(40);
	
	
	angleArc.setCircleResolution(360);
	angleArc.setFilled(true);
	angleArc.setColor(ofColor(240, 130, 10));

}

//--------------------------------------------------------------
void ofApp::update(){

	if (!ofGetMousePressed()) {//press the mouse to stop the angle from incrementing.
		angle+=rotationSpeed;//at each update the angle get's incremented 
	}
	 //if the angle is more than or equal to two PI (a full rotation measured in Radians) then make it zero.
	angle = ofWrap(angle, 0, TWO_PI);
	
	//here we get the sine and cosine values for the angle
	cosine=cos(angle);
	sine=sin(angle);
	tangent=tan(angle);
	
	point = { cosine * radius, sine * radius, 0 };//here we set the cyan circle position
	
	//this is just to draw the arc that represents the angle 
	angleArc.clear();
	angleArc.arc( 0,  0, radius * 0.5f, radius * 0.5f, 0, ofRadToDeg(angle));
	angleArc.lineTo(0,0);
	angleArc.close();
}

//--------------------------------------------------------------
void ofApp::draw(){
	//we translate everything to the center of the circle so it's easier to draw and understand.
	ofTranslate(center);
	
	//draw the yellow circle
	ofSetColor(240, 230, 10);
	ofDrawCircle(0,0, radius);
	
	//draw the cyan circle that's moving over the yellow circle's perimeter 
	ofSetColor(0, 140, 255);
	ofDrawCircle(point, 10);
	
	//draw the orange arc that represents the angle
	angleArc.draw();
	
	
	//draw the axis lines
	ofSetColor(20);
	ofSetLineWidth(1);
	ofDrawLine(-radius * 1.3f, 0, radius * 2, 0);
	ofDrawLine(0, -radius * 2, 0, radius * 1.3f);
	
	ofSetColor(180);
	ofDrawLine(0, -radius , radius * 2, -radius);
	ofDrawLine(0,  radius , radius * 2,  radius);
	ofDrawLine(-radius, 0 ,-radius,  -radius * 2);
	ofDrawLine( radius, 0 , radius,  -radius * 2);
	
	//draw the pink bars
	ofSetColor(255, 0, 127);
	ofDrawRectangle(0, -radius * 2, cosine * radius, 20);
	ofDrawRectangle(radius * 2, 0, -20, sine * radius);
	
	
	ofSetLineWidth(3);
	
	
	//draw lines from the cyan ball to the pink bars
	ofSetColor(255, 0, 50);
	ofDrawLine(point.x, point.y, cosine * radius, -radius * 2);
	ofDrawLine(point.x, point.y, radius * 2, sine * radius);
	
	//draw the line from the yellow circle's center to the cyan circle 
	ofSetColor(0, 127, 255);
	ofDrawLine(0, 0, point.x, point.y);
	

	//draw the dark lines inside the yellow circle that represent the distance from the axis to the cyan circle
	ofSetColor(40);
	ofDrawLine(0, sine * radius, cosine * radius, sine * radius);
	ofDrawLine(cosine * radius, 0, cosine * radius, sine * radius);
	

	// draw the info text
	ofSetColor(10);
	ofDrawBitmapString("Angle (RAD): " + ofToString(angle), 3,-3);
	ofSetColor(30);
	ofDrawBitmapString("Angle (DEG): " + ofToString(ofRadToDeg(angle)), 3,20);
	
	ofDrawBitmapString("Radius: " + ofToString(radius), -radius +20, -3);	
	
	ofSetColor(220);
	ofDrawBitmapString("Angle Sine: " + ofToString(sine), radius *2 +3, 0);
	ofDrawBitmapString("sine x radius: " + ofToString(sine * radius), radius *2 +3, 15	);
	
	ofDrawBitmapString("Angle cosine: " + ofToString(cosine), 0, -radius *2 -20);
	ofDrawBitmapString("cosine x radius: " + ofToString(cosine * radius), 0, -radius *2 -5 );

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
void ofApp::checkMouse(float x, float y){
	glm::vec2 mousePos(x-center.x, y-center.y);
	
	if (glm::length(mousePos) < radius ) {//if the mouse is inside the circle
		// get the angle between the line from the mouse to the center of the circle and the positive x axis
		angle = atan2( mousePos.y , mousePos.x);
		// wrap the angle so it is always between 0 and TWO_PI (one full rotation)
		angle = ofWrap(angle, 0, TWO_PI);
	}
}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	checkMouse(x,y);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	checkMouse(x,y);
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
