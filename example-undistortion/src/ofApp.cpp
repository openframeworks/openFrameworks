#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
	ofSetVerticalSync(true);
	cam.setup(640, 480);
	calibration.setFillFrame(true); // true by default
	calibration.load("mbp-2011-isight.yml");
	imitate(undistorted, cam);
}

void ofApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		calibration.undistort(toCv(cam), toCv(undistorted));
		undistorted.update();
	}
}

void ofApp::draw() {
	ofSetColor(255);
	cam.draw(0, 0);
	undistorted.draw(640, 0);
}
