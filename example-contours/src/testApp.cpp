#include "testApp.h"

void testApp::setup() {
	cam.initGrabber(640, 480);
}

void testApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		contourFinder.setThreshold(ofMap(mouseX, 0, ofGetWidth(), 0, 255));
		contourFinder.findContours(cam);
	}
}

void testApp::draw() {
	cam.draw(0, 0);	
	contourFinder.draw();
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
}
