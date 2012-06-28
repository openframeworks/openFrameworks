#include "testApp.h"

using namespace ofxCv;
using namespace cv;

/*
	A classifier helper should handle the initial resizing for you. It should
	have two "preset" modes for very accurate face finding and very fast.
	It should also do automatic drawing, and give you OF objects instead of CV
	objects.
*/

void testApp::setup() {
	classifier.load(ofToDataPath("haarcascade_frontalface_alt2.xml"));
	
	scaleFactor = .25;
	cam.initGrabber(640, 480);
	
	// shouldn't need to allocate, resize should do this for us
	graySmall.allocate(cam.getWidth() * scaleFactor, cam.getHeight() * scaleFactor, OF_IMAGE_GRAYSCALE);
}

void testApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		convertColor(cam, gray, CV_RGB2GRAY);
		resize(gray, graySmall);
		Mat graySmallMat = toCv(graySmall);
		if(ofGetMousePressed()) {
			equalizeHist(graySmallMat, graySmallMat);
		}
		graySmall.update();
		
		classifier.detectMultiScale(graySmallMat, objects, 1.06, 1,
			//CascadeClassifier::DO_CANNY_PRUNING |
			//CascadeClassifier::FIND_BIGGEST_OBJECT |
			//CascadeClassifier::DO_ROUGH_SEARCH |
			0);
	}
}

void testApp::draw() {
	if(ofGetKeyPressed()) {
		graySmall.draw(0, 0, 640, 480);
	} else {
		cam.draw(0, 0);
	}
	
	ofNoFill();
	
	ofScale(1 / scaleFactor, 1 / scaleFactor);
	for(int i = 0; i < objects.size(); i++) {
		ofRect(toOf(objects[i]));
	}
	
	ofDrawBitmapString(ofToString(objects.size()), 10, 20);
}
