#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
	ofSetVerticalSync(true);
	ofSetFrameRate(120);
	
	objectFinder.setup(ofToDataPath("haarcascade_frontalface_default.xml"));
	objectFinder.setPreset(ObjectFinder::Fast);
	cam.setup(640, 480);
	cropped.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);
}

void ofApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		objectFinder.update(cam);
		if(objectFinder.size() > 0) {
			cv::Rect roi = toCv(objectFinder.getObject(0));
			Mat camMat = toCv(cam);
			Mat croppedCamMat(camMat, roi);
			resize(croppedCamMat, cropped);
			cropped.update();
		}
	}
}

void ofApp::draw() {
	cropped.draw(0, 0);
}
