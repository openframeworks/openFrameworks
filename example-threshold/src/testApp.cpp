#include "testApp.h"

using namespace cv;
using namespace ofxCv;

void testApp::setup() {
	cam.initGrabber(640, 480);
	thresh.allocate(640, 480, OF_IMAGE_GRAYSCALE);
}

void testApp::update() {
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

void testApp::draw() {
	cam.draw(0, 0);
	thresh.draw(640, 0);
}
