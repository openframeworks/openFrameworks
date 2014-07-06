#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
	cam.initGrabber(640, 480);
	background.setLearningTime(900);
	background.setThresholdValue(10);
}

void ofApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		background.update(cam, thresholded);
		thresholded.update();
	}
}

void ofApp::draw() {
	cam.draw(0, 0);
	thresholded.draw(640, 0);
}

void ofApp::keyPressed(int key) {
	if(key == ' ') {
		background.reset();
	}
}