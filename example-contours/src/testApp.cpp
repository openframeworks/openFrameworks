#include "testApp.h"

void testApp::setup() {
	ofSetVerticalSync(true);
	cam.initGrabber(640, 480);
	contourFinder.setMinAreaRadius(10);
	contourFinder.setMaxAreaRadius(80);
	//contourFinder.setInvert(true); // find black instead of white
}

void testApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		contourFinder.setThreshold(ofMap(mouseX, 0, ofGetWidth(), 0, 255));
		contourFinder.findContours(cam);
	}
}

void testApp::draw() {
	ofSetColor(255);
	cam.draw(0, 0);	
	
	ofSetColor(255);
	contourFinder.draw();
	
	ofNoFill();
	ofSetLineWidth(2);
	int n = contourFinder.size();
	for(int i = 0; i < n; i++) {
		ofSetColor(0);
		ofRect(toOf(contourFinder.getBoundingRect(i)));
		
		ofSetColor(cyanPrint);
		ofPolyline minAreRect = toOf(contourFinder.getMinAreaRect(i));
		minAreRect.draw();
		
		ofSetColor(magentaPrint);
		cv::RotatedRect ellipse = contourFinder.getFitEllipse(i);
		ofPushMatrix();
		ofVec2f ellipseCenter = toOf(ellipse.center);
		ofVec2f ellipseSize = toOf(ellipse.size);
		ofTranslate(ellipseCenter.x, ellipseCenter.y);
		ofRotate(ellipse.angle);
		ofEllipse(0, 0, ellipseSize.x, ellipseSize.y);
		ofPopMatrix();
		
		ofSetColor(yellowPrint);
		ofPolyline convexHull = toOf(contourFinder.getConvexHull(i));
		convexHull.draw();
		
		ofVec2f centroid = toOf(contourFinder.getCentroid(i));
		ofVec2f average = toOf(contourFinder.getAverage(i));
		ofVec2f center = toOf(contourFinder.getCenter(i));
		double area = contourFinder.getContourArea(i);
		double length = contourFinder.getArcLength(i);
		drawHighlightString(ofToString((int) area) + "/" + ofToString((int) length), centroid);
		
		ofSetColor(cyanPrint);
		ofCircle(centroid, 1);
		ofSetColor(magentaPrint);
		ofCircle(average, 1);
		ofSetColor(yellowPrint);
		ofCircle(center, 1);
		
		ofVec2f balance = toOf(contourFinder.getBalance(i));
		ofPushMatrix();
		ofTranslate(centroid.x, centroid.y);
		ofScale(5, 5);
		ofLine(0, 0, balance.x, balance.y);
		ofPopMatrix();
	}
	
	ofSetColor(255);
	drawHighlightString(ofToString((int) ofGetFrameRate()), 10, 20);
}
