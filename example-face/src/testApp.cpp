#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	finder.setup("haarcascade_frontalface_alt2.xml");
	finder.setRescale(.25);
	cam.initGrabber(640, 480);
}

void testApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		finder.update(cam);
	}
}

void testApp::draw() {
	cam.draw(0, 0);
	ofNoFill();
	for(int i = 0; i < finder.size(); i++) {
		ofRect(finder.getObject(i));
	}
	ofDrawBitmapString(ofToString(finder.size()), 10, 20);
}
