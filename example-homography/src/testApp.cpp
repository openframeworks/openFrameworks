#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	ofSetVerticalSync(true);
	
	left.loadImage("left.jpg");
	right.loadImage("right.jpg");
	imitate(warpedColor, right);
	
	movingPoint = false;
	saveMatrix = false;
	homographyReady = false;
	
	// load the previous homography if it's available
	ofFile previous("homography.yml");
	if(previous.exists()) {
		FileStorage fs(ofToDataPath("homography.yml"), FileStorage::READ);
		fs["homography"] >> homography;
		homographyReady = true;
	}
}

void testApp::update() {
	if(leftPoints.size() >= 4) {
		vector<Point2f> srcPoints, dstPoints;
		for(int i = 0; i < leftPoints.size(); i++) {
			srcPoints.push_back(Point2f(rightPoints[i].x - left.getWidth(), rightPoints[i].y));
			dstPoints.push_back(Point2f(leftPoints[i].x, leftPoints[i].y));
		}
		
		// generate a homography from the two sets of points
		homography = findHomography(Mat(srcPoints), Mat(dstPoints));
		homographyReady = true;
		
		if(saveMatrix) {
			FileStorage fs(ofToDataPath("homography.yml"), FileStorage::WRITE);
			fs << "homography" << homography;
			saveMatrix = false;
		}
	}
	
	if(homographyReady) {
		// this is how you warp one ofImage into another ofImage given the homography matrix
		// CV INTER NN is 113 fps, CV_INTER_LINEAR is 93 fps
		warpPerspective(right, warpedColor, homography, CV_INTER_LINEAR);
		warpedColor.update();
	}
}

void drawPoints(vector<ofVec2f>& points) {
	ofNoFill();
	for(int i = 0; i < points.size(); i++) {
		ofCircle(points[i], 10);
		ofCircle(points[i], 1);
	}
}

void testApp::draw() {
	
	ofSetColor(255);
	left.draw(0, 0);
	right.draw(left.getWidth(), 0);
	if(homographyReady) {
		ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
		ofSetColor(255, 128);
		warpedColor.draw(0, 0);
		ofDisableBlendMode();
	}
	
	ofSetColor(ofColor::red);
	drawPoints(leftPoints);
	ofSetColor(ofColor::blue);
	drawPoints(rightPoints);
	ofSetColor(128);
	for(int i = 0; i < leftPoints.size(); i++) {
		ofLine(leftPoints[i], rightPoints[i]);
	}
	
	ofSetColor(255);
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
}

bool testApp::movePoint(vector<ofVec2f>& points, ofVec2f point) {
	for(int i = 0; i < points.size(); i++) {
		if(points[i].distance(point) < 20) {
			movingPoint = true;
			curPoint = &points[i];
			return true;
		}
	}
	return false;
}

void testApp::mousePressed(int x, int y, int button) {
	ofVec2f cur(x, y);
	ofVec2f rightOffset(left.getWidth(), 0);
	if(!movePoint(leftPoints, cur) && !movePoint(rightPoints, cur)) {
		if(x > left.getWidth()) {
			cur -= rightOffset;
		}
		leftPoints.push_back(cur);
		rightPoints.push_back(cur + rightOffset);
	}
}

void testApp::mouseDragged(int x, int y, int button) {
	if(movingPoint) {
		curPoint->set(x, y);
	}
}

void testApp::mouseReleased(int x, int y, int button) {
	movingPoint = false;
}

void testApp::keyPressed(int key) {
	if(key == ' ') {
		saveMatrix = true;
	}
}