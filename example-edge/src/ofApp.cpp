#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
	cam.initGrabber(640, 480);
}

void ofApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		convertColor(cam, gray, CV_RGB2GRAY);
		Canny(gray, edge, mouseX, mouseY, 3);
		edge.update();
	}
}

void ofApp::draw() {
	cam.draw(0, 0);
	edge.draw(640, 0);
}
