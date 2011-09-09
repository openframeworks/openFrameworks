#include "testApp.h"

void testApp::setup() {
	cam.initGrabber(640, 480);
	thresholded.allocate(640, 480, OF_IMAGE_GRAYSCALE);
	background.setLearningTime(9000);
}

void testApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		background.setThresholdValue(mouseX);
		Mat thresholdedMat = toCv(thresholded);
		background.update(toCv(cam), thresholdedMat);
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