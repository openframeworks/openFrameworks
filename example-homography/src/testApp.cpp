#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	//ofSetVerticalSync(true);
	
	depth.loadImage("depth.png");
	color.loadImage("color.png");
	imitate(warpedColor, color);
	
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
	if(depthPoints.size() > 4) {
		vector<Point2f> srcPoints, dstPoints;
		for(int i = 0; i < depthPoints.size(); i++) {
			srcPoints.push_back(Point2f(colorPoints[i].x - depth.getWidth(), colorPoints[i].y));
			dstPoints.push_back(Point2f(depthPoints[i].x, depthPoints[i].y));
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
		warpPerspective(color, warpedColor, homography, CV_INTER_LINEAR);
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
	depth.draw(0, 0);
	color.draw(depth.getWidth(), 0);
	if(homographyReady) {
		ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
		ofSetColor(255, 128);
		warpedColor.draw(0, 0);
		ofDisableBlendMode();
	}
	
	ofSetColor(ofColor::red);
	drawPoints(depthPoints);
	ofSetColor(ofColor::blue);
	drawPoints(colorPoints);
	ofSetColor(128);
	for(int i = 0; i < depthPoints.size(); i++) {
		ofLine(depthPoints[i], colorPoints[i]);
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
	ofVec2f colorOffset(depth.getWidth(), 0);
	if(!movePoint(depthPoints, cur) && !movePoint(colorPoints, cur)) {
		if(x > depth.getWidth()) {
			cur -= colorOffset;
		}
		depthPoints.push_back(cur);
		colorPoints.push_back(cur + colorOffset);
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