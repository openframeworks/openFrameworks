#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	cam.initGrabber(640, 480);
}

void testApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		convertColor(cam, gray, CV_RGB2GRAY);
		Canny(gray, edge, mouseX, mouseY, 3);
		edge.update();
	}
}

void testApp::draw() {
	cam.draw(0, 0);
	edge.draw(640, 0);
}
