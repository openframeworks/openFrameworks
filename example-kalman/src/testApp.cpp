#include "testApp.h"

using namespace ofxCv;
using namespace cv;

// based on code from:
// http://www.morethantechnical.com/2011/06/17/simple-kalman-filter-for-tracking-using-opencv-2-2-w-code/

void testApp::setup() {
	ofSetVerticalSync(true);
	
	KF.init(4, 2, 0);
	
	KF.transitionMatrix = *(Mat_<float>(4, 4) <<
		1,0,1,0,
		0,1,0,1,
		0,0,1,0,
		0,0,0,1);
		
	measurement = Mat_<float>::zeros(2,1);
		
	KF.statePre.at<float>(0) = mouseX;
	KF.statePre.at<float>(1) = mouseY;
	KF.statePre.at<float>(2) = 0;
	KF.statePre.at<float>(3) = 0;
	
	setIdentity(KF.measurementMatrix);
	setIdentity(KF.processNoiseCov, Scalar::all(1e-4));
	setIdentity(KF.measurementNoiseCov, Scalar::all(1e-1));
	setIdentity(KF.errorCovPost, Scalar::all(.1));
}

void testApp::update() {
	// First predict, to update the internal statePre variable
	Mat prediction = KF.predict();
	cv::Point predictPt(prediction.at<float>(0),prediction.at<float>(1));
	point = toOf(predictPt);
	predicted.addVertex(point);
	
	// Get mouse point
	measurement(0) = mouseX;
	measurement(1) = mouseY;
	
	cv::Point measPt(measurement(0),measurement(1));
	line.addVertex(toOf(measPt));
	
	// The "correct" phase that is going to use the predicted value and our measurement
	Mat estimated = KF.correct(measurement);
	cv::Point statePt(estimated.at<float>(0),estimated.at<float>(1));
}

void testApp::draw() {
	ofBackground(0);
	ofSetColor(255);
	//line.draw();
	ofSetColor(ofColor::red);
	predicted.draw();
	ofNoFill();
	ofCircle(point, 100);
}
