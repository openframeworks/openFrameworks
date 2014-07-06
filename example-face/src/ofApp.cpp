#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
	ofSetVerticalSync(true);
	ofSetFrameRate(120);
	finder.setup("haarcascade_frontalface_alt2.xml");
	finder.setPreset(ObjectFinder::Fast);
	cam.initGrabber(640, 480);
}

void ofApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		finder.update(cam);
	}
}

void ofApp::draw() {
	cam.draw(0, 0);
	finder.draw();
	ofDrawBitmapStringHighlight(ofToString(finder.size()), 10, 20);
}
