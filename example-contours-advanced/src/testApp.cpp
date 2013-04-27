#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	cam.initGrabber(640, 480);
	contourFinder.setMinAreaRadius(10);
	contourFinder.setMaxAreaRadius(150);
	//contourFinder.setInvert(true); // find black instead of white
	trackingColorMode = TRACK_COLOR_RGB;
}

void testApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		threshold = ofMap(mouseX, 0, ofGetWidth(), 0, 255);
		contourFinder.setThreshold(threshold);
		contourFinder.findContours(cam);
	}
}

void testApp::draw() {
	ofSetColor(255);
	cam.draw(0, 0);	
	
	ofSetLineWidth(2);
	contourFinder.draw();
	
	ofNoFill();
	int n = contourFinder.size();
	for(int i = 0; i < n; i++) {
		// smallest rectangle that fits the contour
		ofSetColor(cyanPrint);
		ofPolyline minAreRect = toOf(contourFinder.getMinAreaRect(i));
		minAreRect.draw();
		
		// ellipse that best fits the contour
		ofSetColor(magentaPrint);
		cv::RotatedRect ellipse = contourFinder.getFitEllipse(i);
		ofPushMatrix();
		ofVec2f ellipseCenter = toOf(ellipse.center);
		ofVec2f ellipseSize = toOf(ellipse.size);
		ofTranslate(ellipseCenter.x, ellipseCenter.y);
		ofRotate(ellipse.angle);
		ofEllipse(0, 0, ellipseSize.x, ellipseSize.y);
		ofPopMatrix();
		
		// minimum area circle that encloses the contour
		ofSetColor(cyanPrint);
		float circleRadius;
		ofVec2f circleCenter = toOf(contourFinder.getMinEnclosingCircle(i, circleRadius));
		ofCircle(circleCenter, circleRadius);
		
		// convex hull of the contour
		ofSetColor(yellowPrint);
		ofPolyline convexHull = toOf(contourFinder.getConvexHull(i));
		convexHull.draw();
		
		// defects of the convex hull
		vector<cv::Vec4i> defects = contourFinder.getConvexityDefects(i);
		for(int j = 0; j < defects.size(); j++) {
			ofLine(defects[j][0], defects[j][1], defects[j][2], defects[j][3]);
		}
		
		// some different styles of contour centers
		ofVec2f centroid = toOf(contourFinder.getCentroid(i));
		ofVec2f average = toOf(contourFinder.getAverage(i));
		ofVec2f center = toOf(contourFinder.getCenter(i));
		ofSetColor(cyanPrint);
		ofCircle(centroid, 1);
		ofSetColor(magentaPrint);
		ofCircle(average, 1);
		ofSetColor(yellowPrint);
		ofCircle(center, 1);
		
		// you can also get the area and perimeter using ofPolyline:
		// ofPolyline::getArea() and ofPolyline::getPerimeter()
		double area = contourFinder.getContourArea(i);
		double length = contourFinder.getArcLength(i);		
		
		// balance is useful for detecting when a shape has an "arm" sticking out
		// if balance.length() is small, the shape is more symmetric: like I, O, X...
		// if balance.length() is large, the shape is less symmetric: like L, P, F...
		ofVec2f balance = toOf(contourFinder.getBalance(i));
		ofPushMatrix();
		ofTranslate(centroid.x, centroid.y);
		ofScale(5, 5);
		ofLine(0, 0, balance.x, balance.y);
		ofPopMatrix();
	}

	ofSetColor(255);
	drawHighlightString(ofToString((int) ofGetFrameRate()) + " fps", 10, 10);
	drawHighlightString(ofToString((int) threshold) + " threshold", 10, 30);
	drawHighlightString(trackingColorMode == TRACK_COLOR_RGB ? "RGB tracking" : "hue tracking", 10, 50);
	
	ofTranslate(8, 75);
	ofFill();
	ofSetColor(0);
	ofRect(-3, -3, 64+6, 64+6);
	ofSetColor(targetColor);
	ofRect(0, 0, 64, 64);
}

void testApp::mousePressed(int x, int y, int button) {
	targetColor = cam.getPixelsRef().getColor(x, y);
	contourFinder.setTargetColor(targetColor, trackingColorMode);
}

void testApp::keyPressed(int key) {
	if(key == 'h') {
		trackingColorMode = TRACK_COLOR_HS;
	}
	if(key == 'r') {
		trackingColorMode = TRACK_COLOR_RGB;
	}
	contourFinder.setTargetColor(targetColor, trackingColorMode);
}