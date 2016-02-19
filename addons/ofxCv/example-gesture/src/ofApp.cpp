#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
	ofSetVerticalSync(true);
	ofEnableSmoothing();
	ofEnableAlphaBlending();
	ofSetLineWidth(3);
	ofSetFrameRate(120);
}

void ofApp::update() {
	
}

void ofApp::draw() {
	ofBackground(0);
	ofSetColor(255, 64);
	polyline.draw();

	switch(recognizer.getGestureType()) {
		case Recognizer::GESTURE_LINE: ofSetColor(magentaPrint); break;
		case Recognizer::GESTURE_ARC: ofSetColor(cyanPrint); break;
	}
	if(recognizer.getFitError() < .5) {
		recognizer.getPolyline().draw();
	}
}

void ofApp::mousePressed(int x, int y, int button) {
	polyline.clear();
}

void ofApp::mouseDragged(int x, int y, int button) {
	polyline.addVertex(x, y);
	recognizer.update(polyline);
}
