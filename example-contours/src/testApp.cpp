#include "testApp.h"

void testApp::setup() {
	cam.initGrabber(640, 480);
	thresh.allocate(640, 480, OF_IMAGE_GRAYSCALE);
}

void testApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		contourFinder.findContours(cam);
	
		convertColor(cam, thresh, CV_RGB2GRAY);
		float thresholdValue = ofMap(mouseX, 0, ofGetWidth(), 0, 255);
		ofxCv::threshold(thresh, thresholdValue);
		Mat testMat;
		ofxCv::threshold(testMat, thresholdValue);
		Mat threshMat = toCv(thresh);
		findContours(threshMat, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
		thresh.update();
		
		// to implement in ContourFinder:
		// drawing
		// holes/no holes
		// autothreshold + OTSU
		// cv::arcLength
		// cv::boundingRect
		// cv::contourArea and filtering
		// cv::convexHull
		// cv::minAreaRect
		// cv::fitEllipse
		// cv::pointPolygonTest
		
		// cv::matchShapes?
		// cv::estimateRigidTransform? subdivision-based estimation for outline-flow?
	}
}

void testApp::draw() {
	cam.draw(0, 0);
	thresh.draw(640, 0);
	
	for(int i = 0; i < contours.size(); i++) {
		ofNoFill();
		ofBeginShape();
		for(int j = 0; j < contours[i].size(); j++) {
			ofVertex(contours[i][j].x, contours[i][j].y);
		}
		ofEndShape(true);
	}
}
