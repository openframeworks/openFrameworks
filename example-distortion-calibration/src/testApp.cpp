#include "testApp.h"

void testApp::setup() {
	ofSetVerticalSync(true);
	cam.initGrabber(640, 480);
	
	imitate(undistorted, cam);
	imitate(previous, cam);;
	imitate(diff, cam);
}

void testApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		if(calibration.size() > 0) {
			calibration.undistort(toCv(cam), toCv(undistorted));
		}
	}
}

void testApp::draw() {
	cam.draw(0, 0);
}
