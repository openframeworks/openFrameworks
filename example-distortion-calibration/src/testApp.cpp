#include "testApp.h"

void testApp::setup() {
	ofSetVerticalSync(true);
	cam.initGrabber(640, 480);
	undistorted.allocate(640, 480, OF_IMAGE_COLOR);
}

void testApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		//calibration.undistort(toCv(cam), toCv(undistorted));
	}
}

void testApp::draw() {
	cam.draw(0, 0);
}
