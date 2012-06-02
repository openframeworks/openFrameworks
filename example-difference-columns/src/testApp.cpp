#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	ofSetVerticalSync(true);
	cam.initGrabber(320, 240);
	
	// imitate() will set up previous and diff
	// so they have the same size and type as cam
	imitate(previous, cam);
	imitate(diff, cam);
}

void testApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		// take the absolute difference of prev and cam and save it inside diff
		absdiff(previous, cam, diff);
		diff.update();
		
		// like ofSetPixels, but more concise and cross-toolkit
		copy(cam, previous);
		
		// this is the key line: get the average of each column
		columnMean = meanCols(diff);
	}
}

void testApp::draw() {
	ofSetColor(255);
	cam.draw(0, 0);
	
	ofTranslate(320, 0);
	diff.draw(0, 0);
	// draw the mean for each channel
	for(int k = 0; k < 3; k++) {
		// use the correct color for each channel
		switch(k) {
			case 0: ofSetColor(ofColor::red); break;
			case 1: ofSetColor(ofColor::blue); break;
			case 2: ofSetColor(ofColor::green); break;
		}
		
		ofNoFill();
		ofBeginShape();
		for(int i = 0; i < columnMean.rows; i++) {
			// Vec3b is one way of storing 24-bit (3 byte) colors
			Vec3b cur = columnMean.at<Vec3b>(i);
			ofVertex(i, cur[k]);
		}
		ofEndShape();
	}
}
