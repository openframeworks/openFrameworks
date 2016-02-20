#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
	cam.setup(640, 480);
	contourFinder.setMinAreaRadius(10);
	contourFinder.setMaxAreaRadius(200);
	trackingColorMode = TRACK_COLOR_RGB;
}

void ofApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		threshold = ofMap(mouseX, 0, ofGetWidth(), 0, 255);
		contourFinder.setThreshold(threshold);
		contourFinder.findContours(cam);
	}
}

void ofApp::draw() {
	ofSetColor(255);
	cam.draw(0, 0);
	contourFinder.draw();
	drawHighlightString(ofToString((int) ofGetFrameRate()) + " fps", 10, 20);
	drawHighlightString(ofToString((int) threshold) + " threshold", 10, 40);
	drawHighlightString(trackingColorMode == TRACK_COLOR_HSV ? "HSV tracking" : "RGB tracking", 10, 60);
	
	ofTranslate(8, 75);
	ofFill();
	ofSetColor(0);
	ofDrawRectangle(-3, -3, 64+6, 64+6);
	ofSetColor(targetColor);
	ofDrawRectangle(0, 0, 64, 64);
}

void ofApp::mousePressed(int x, int y, int button) {
	targetColor = cam.getPixels().getColor(x, y);
	contourFinder.setTargetColor(targetColor, trackingColorMode);
}

void ofApp::keyPressed(int key) {
	if(key == 'h') {
		trackingColorMode = TRACK_COLOR_HSV;
	}
	if(key == 'r') {
		trackingColorMode = TRACK_COLOR_RGB;
	}
	contourFinder.setTargetColor(targetColor, trackingColorMode);
}