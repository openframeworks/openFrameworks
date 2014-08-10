#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
	ofSetVerticalSync(true);
	
	kalman.init(1e-5, 1e-1); // invert of (smoothness, rapidness)
}

void ofApp::update() {
	m.makeIdentityMatrix();
	// generate rotation for demo
	m.rotate(ofMap(mouseX, 0, ofGetWidth(), -180, 180), 1, 0, 0);
	m.rotate(ofMap(mouseY, 0, ofGetHeight(), -180, 180), 0, 1, 0);
	m.rotate(ofMap(ofGetElapsedTimef(), 0, 1, 0, 180), 0, 1, 1);
	
	ofQuaternion q;
	q = m.getRotate();
	kalman.update(q);
	mPredicted.setRotate(kalman.getEstimation());
}

void drawTransparentAxis(float size, int alpha) {
	ofPushStyle();
	ofSetLineWidth(3);
	
	// draw x axis
	ofSetColor(255, 0, 0, alpha);
	ofLine(0, 0, 0, size, 0, 0);
	
	// draw y axis
	ofSetColor(0, 255, 0, alpha);
	ofLine(0, 0, 0, 0, size, 0);
	
	// draw z axis
	ofSetColor(0, 0, 255, alpha);
	ofLine(0, 0, 0, 0, 0, size);
	ofPopStyle();
}

void ofApp::draw() {
	ofBackground(54);
	
	cam.begin();
	
	ofPushMatrix();
	glMultMatrixf((GLfloat*)m.getPtr());
	drawTransparentAxis(200, 128);
	ofPopMatrix();
	
	ofPushMatrix();
	glMultMatrixf((GLfloat*)mPredicted.getPtr());
	ofDrawAxis(200);
	ofPopMatrix();
	
	cam.end();
}
