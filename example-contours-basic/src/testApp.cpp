#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	cam.initGrabber(640, 480);
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
	for(int i = 0; i < contourFinder.size(); i++) {
		vector<cv::Vec4i> defects = contourFinder.getConvexityDefects(i);
		for(int j = 0; j < defects.size(); j++) {
			ofLine(defects[j][0], defects[j][1], defects[j][2], defects[j][3]);
		}
	}
}
