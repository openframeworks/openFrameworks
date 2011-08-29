#include "testApp.h"

// click on a color to select the target
// mouseX is still threshold
// keyboard toggle between hsv and rgb
// add toCv for ofColor

void testApp::setup() {
	cam.initGrabber(640, 480);
	contourFinder.setTargetColor(ofColor(228, 186, 14));
	contourFinder.setMinAreaRadius(10);
	contourFinder.setMaxAreaRadius(200);
}

void testApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		contourFinder.setThreshold(ofMap(mouseX, 0, ofGetWidth(), 0, 255));
		contourFinder.findContours(cam);
	}
}

void testApp::draw() {
	ofSetColor(255);
	cam.draw(0, 0);
	contourFinder.draw();
}
