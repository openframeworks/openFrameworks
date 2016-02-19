#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
	cam.setup(640, 480);
}

void ofApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		convertColor(cam, gray, CV_RGB2GRAY);
		Canny(gray, edge, mouseX, mouseY, 3);
    Sobel(gray, sobel);
    gray.update();
    sobel.update();
		edge.update();
	}
}

void ofApp::draw() {
	cam.draw(0, 0);
  gray.draw(0,480);
	edge.draw(640, 0);
  sobel.draw(640, 480);
}
