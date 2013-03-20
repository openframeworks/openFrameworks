#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	ofSetVerticalSync(true);
	ofSetFrameRate(120);
	
	classifier.load(ofToDataPath("haarcascade_frontalface_alt2.xml"));
	scaleFactor = .5;
	cam.initGrabber(640, 480);
	graySmall.allocate(cam.getWidth() * scaleFactor, cam.getHeight() * scaleFactor, OF_IMAGE_GRAYSCALE);
	cropped.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);
}

void testApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		convertColor(cam, gray, CV_RGB2GRAY);
		resize(gray, graySmall);
		Mat graySmallMat = toCv(graySmall);
		equalizeHist(graySmallMat, graySmallMat);		
		classifier.detectMultiScale(graySmallMat, objects, 1.05, 1,
			CASCADE_DO_CANNY_PRUNING |
			CASCADE_FIND_BIGGEST_OBJECT |
			//CASCADE_DO_ROUGH_SEARCH |
			0);
		if(!objects.empty()) {
			cv::Rect roi = objects[0];
			roi.x /= scaleFactor;
			roi.y /= scaleFactor;
			roi.width /= scaleFactor;
			roi.height /= scaleFactor;
			Mat camMat = toCv(cam);
			Mat croppedCamMat(camMat, roi);
			resize(croppedCamMat, cropped);
			cropped.update();
		}
	}
}

void testApp::draw() {
	cropped.draw(0, 0);
	ofDrawBitmapStringHighlight(ofToString(objects.size()) + " object @ " + ofToString((int) ofGetFrameRate()) + "fps", 10, 20);
}
