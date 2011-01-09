#include "testApp.h"

void testApp::setup() {
}

void testApp::update() {
}

void testApp::draw() {
	float ox = ofGetWidth() / 2;
	float oy = ofGetHeight() / 2;
	float tx = mouseX - ox;
	float ty = mouseY - oy;
	
	ofBackground(0, 0, 0);
	ofSetColor(255);
	ofTranslate(ox, oy);
	ofLine(0, 0, tx, ty);
	
	ofTranslate(tx, ty);
	ofDrawBitmapString(ofToString((int) tx) + ", " + ofToString((int) ty), 10, 20);
	float angle = atan2f(ty, tx);
	ofDrawBitmapString(ofToString(angle / PI, 2) + " PI", 10, 40);
	ofDrawBitmapString(ofToString(ofRadToDeg(angle), 2), 10, 60);
}

void testApp::keyPressed(int key) {
}

void testApp::keyReleased(int key) {
}

void testApp::mouseMoved(int x, int y) {
}

void testApp::mouseDragged(int x, int y, int button) {
}

void testApp::mousePressed(int x, int y, int button) {
}

void testApp::mouseReleased(int x, int y, int button) {
}

void testApp::windowResized(int w, int h) {
}

