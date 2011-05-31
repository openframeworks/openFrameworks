#include "testApp.h"

/*
	A classifier helper should handle the initial resizing for you. It should
	have two "preset" modes for very accurate face finding and very fast.
	It should also do automatic drawing, and give you OF objects instead of CV
	objects.
*/

void testApp::setup() {
	classifier.load(ofToDataPath("haarcascade_frontalface_default.xml"));
	
	scaleFactor = .25;
	cam.initGrabber(640, 480);
	gray.allocate(cam.getWidth(), cam.getHeight(), OF_IMAGE_GRAYSCALE);
	graySmall.allocate(cam.getWidth() * scaleFactor, cam.getHeight() * scaleFactor, OF_IMAGE_GRAYSCALE);
}

void testApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		convertColor(cam.getPixelsRef(), gray, CV_RGB2GRAY);
		resize(gray, graySmall);
		
		Mat graySmallMat = toCv(graySmall);
		classifier.detectMultiScale(graySmallMat, objects, 1.06, 3,
			CascadeClassifier::DO_CANNY_PRUNING |
			CascadeClassifier::FIND_BIGGEST_OBJECT |
			CascadeClassifier::DO_ROUGH_SEARCH);
	}
}

void testApp::draw() {
	cam.draw(0, 0);
	
	ofNoFill();
	ofScale(1 / scaleFactor, 1 / scaleFactor);
	for(int i = 0; i < objects.size(); i++) {
		ofRect(toOf(objects[i]));
	}
}
