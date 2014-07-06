#include "ofApp.h"

using namespace cv;
using namespace ofxCv;

void ofApp::setup() {
	cam.initGrabber(640, 480);
	thresh.allocate(640, 480, OF_IMAGE_GRAYSCALE);
}

void ofApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		convertColor(cam, thresh, CV_RGB2GRAY);
		if(ofGetMousePressed()) {
			autothreshold(thresh);
		} else {
			float thresholdValue = ofMap(mouseX, 0, ofGetWidth(), 0, 255);
			threshold(thresh, thresholdValue);
		}
		thresh.update();
	}
}

void ofApp::draw() {
	cam.draw(0, 0);
	thresh.draw(640, 0);
}
