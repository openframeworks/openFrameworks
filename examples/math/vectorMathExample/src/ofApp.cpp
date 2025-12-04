#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);

	nPts = 0;
	rotateAmount = {0, 0, 1};
	speedOfRotation = ofDegToRad(0.5f);

	ofBackground(255,255,255);

	glm::vec3 center(ofGetWidth()/2, ofGetHeight()/2, 0);

	xAxisMin = {-100,   0,    0};
	xAxisMax = { 100,   0,    0};
	yAxisMin = {   0,-100,    0};
	yAxisMax = {   0, 100,    0};
	zAxisMin = {   0,   0, -100};
	zAxisMax = {   0,   0,  100};

	xAxisMin += center;
	xAxisMax += center;
	yAxisMin += center;
	yAxisMax += center;
	zAxisMin += center;
	zAxisMax += center;

	bDrawnAnything = false;     // added this cause theo saw this example
								// and was like "how do I tell zach that a rotating
								// cross is not that interesting ??"  before realizing
								// that one should draw :)

}

//--------------------------------------------------------------
void ofApp::update(){

	 // rotation occurs around 0,0,0 position.
	 // since 0,0,0 is the top left corner, we need to:
	 // (A) move over,
	 // (B) rotate
	 // (C) move back
	 // this could all be one for loop, but broken out here for
	 // clarity on the steps:

	 glm::vec3 center(ofGetWidth()/2, ofGetHeight()/2,0);

	 // move the points so that their center (ofGetW/2, ofGetH/2) is at 0,0,0
	 for (int i = 0; i < nPts; i++){
		pts[i]-= center;
	 }

	 // rotate the points
	 for (int i = 0; i < nPts; i++){
		 pts[i] = glm::rotate(pts[i],speedOfRotation, rotateAmount);
	 }

	 // move them back
	 for (int i = 0; i < nPts; i++){
		pts[i]+= center;
	 }


	 xAxisMin -= center;
	 xAxisMax -= center;
	 yAxisMin -= center;
	 yAxisMax -= center;
	 zAxisMin -= center;
	 zAxisMax -= center;


	 xAxisMin = glm::rotate(xAxisMin, speedOfRotation, rotateAmount);
	 xAxisMax = glm::rotate(xAxisMax, speedOfRotation, rotateAmount);
	 yAxisMin = glm::rotate(yAxisMin, speedOfRotation, rotateAmount);
	 yAxisMax = glm::rotate(yAxisMax, speedOfRotation, rotateAmount);
	 zAxisMin = glm::rotate(zAxisMin, speedOfRotation, rotateAmount);
	 zAxisMax = glm::rotate(zAxisMax, speedOfRotation, rotateAmount);

	 xAxisMin += center;
	 xAxisMax += center;
	 yAxisMin += center;
	 yAxisMax += center;
	 zAxisMin += center;
	 zAxisMax += center;
}

//--------------------------------------------------------------
void ofApp::draw(){

	 ofSetHexColor(0x000000);
	 ofNoFill();
	 ofBeginShape();
	 for (int i = 0; i < nPts; i++){
		 ofVertex(pts[i].x, pts[i].y);
	 }
	 ofEndShape();

	 string info = "speed of rotation (a/z): " + ofToString(speedOfRotation,3) + "\n" +
				  "rotateAmount in x (s/x): " + ofToString(rotateAmount.x,3) + "\n" +
				  "rotateAmount in y (d/c): " + ofToString(rotateAmount.y,3) + "\n" +
				  "rotateAmount in z (f/v): " + ofToString(rotateAmount.z,3);

	 ofFill();
	 ofSetHexColor(0xE5A93F);
	 ofDrawRectangle(10,10,300,70);
	 ofSetHexColor(0x000000);
	 ofDrawBitmapString(info,30,30);


	 ofSetColor(50,50,255);
	 ofNoFill();
	 ofBeginShape();
		ofVertex(xAxisMin.x, xAxisMin.y, xAxisMin.z);
		ofVertex(xAxisMax.x, xAxisMax.y, xAxisMax.z);
	 ofEndShape();
	 ofBeginShape();
		ofVertex(yAxisMin.x, yAxisMin.y, yAxisMin.z);
		ofVertex(yAxisMax.x, yAxisMax.y, yAxisMax.z);
	 ofEndShape();
	 ofBeginShape();
		ofVertex(zAxisMin.x, zAxisMin.y, zAxisMin.z);
		ofVertex(zAxisMax.x, zAxisMax.y, zAxisMax.z);
	 ofEndShape();


	if (bDrawnAnything == false){
		ofSetColor(0,0,0);
		ofDrawBitmapString("draw something!", ofGetWidth()/2 + 100,ofGetHeight()/2 + 20);
	}

}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){

	switch (key){
		case 'a':
			speedOfRotation += 0.001f;
			break;
		case 'z':
			speedOfRotation -= 0.001f;
			break;
		case 's':
			rotateAmount.x += 0.05f;
			break;
		case 'x':
			 rotateAmount.x -= 0.05f;
			break;
		case 'd':
			rotateAmount.y += 0.05f;
			break;
		case 'c':
			 rotateAmount.y -= 0.05f;
			break;
		case 'f':
			rotateAmount.z += 0.05f;
			break;
		case 'v':
			 rotateAmount.z -= 0.05f;
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased  (int key){
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

	 bDrawnAnything = true;

	 if (nPts < MAX_N_PTS){
		pts[nPts].x = x;
		pts[nPts].y = y;
		nPts++;
	 }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	 nPts = 0;
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
