#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	cam.initGrabber(640, 480);
	background.setLearningTime(900);
	background.setThresholdValue(10);
}

void testApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		background.update(cam, thresholded);
		thresholded.update();
	}
}

void testApp::draw() {
	cam.draw(0, 0);
	thresholded.draw(640, 0);
}

void testApp::keyPressed(int key) {
	if(key == ' ') {
		background.reset();
	}
}