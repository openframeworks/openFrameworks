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
		
		// mean() returns a Scalar. it's a cv:: function so we have to pass a Mat
		diffMean = mean(toCv(diff));
		
		// you can only do math between Scalars,
		// but it's easy to make a Scalar from an int (shown here)
		diffMean *= Scalar(50);
	}
}

void testApp::draw() {
	ofSetColor(255);
	cam.draw(0, 0);
	diff.draw(320, 0);
	
	// use the [] operator to get elements from a Scalar
	float diffRed = diffMean[0];
	float diffGreen = diffMean[1];
	float diffBlue = diffMean[2];
	
	ofSetColor(255, 0, 0);
	ofRect(0, 0, diffRed, 10);
	ofSetColor(0, 255, 0);
	ofRect(0, 15, diffGreen, 10);
	ofSetColor(0, 0, 255);
	ofRect(0, 30, diffBlue, 10);
}
