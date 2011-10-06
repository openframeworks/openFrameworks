#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	cam.initGrabber(640, 480);
	contourFinder.setMinAreaRadius(10);
	contourFinder.setMaxAreaRadius(200);
	unwarped.allocate(150, 100, OF_IMAGE_COLOR);
}

void testApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		threshold = ofMap(mouseX, 0, ofGetWidth(), 0, 255);
		contourFinder.setThreshold(threshold);
		contourFinder.findContours(cam);
		
		int n = contourFinder.size();
		quads.clear();
		quads.resize(n);
		for(int i = 0; i < n; i++) {
			quads[i] = contourFinder.getFitQuad(i);
			
			// convert integer image coordinates Point2i to unwarp positions Point2f
			vector<Point2f> warpPoints;
			copy(quads[i].begin(), quads[i].end(), back_inserter(warpPoints));
			unwarpPerspective(cam, unwarped, warpPoints);
			unwarped.update();
		}
	}
}

void testApp::draw() {
	ofSetColor(255);
	cam.draw(0, 0);	
	
	ofSetLineWidth(2);
	contourFinder.draw();
	
	ofNoFill();
	ofSetColor(magentaPrint);
	for(int i = 0; i < quads.size(); i++) {
		toOf(quads[i]).draw();
	}

	ofSetColor(255);
	drawHighlightString(ofToString((int) ofGetFrameRate()) + " fps", 10, 20);
	drawHighlightString(ofToString((int) threshold) + " threshold", 10, 40);
	
	ofTranslate(8, 75);
	ofFill();
	ofSetColor(0);
	ofRect(-3, -3, 64+6, 64+6);
	ofSetColor(targetColor);
	ofRect(0, 0, 64, 64);
	
	ofSetColor(255);
	unwarped.draw(0, 70);
}

void testApp::mousePressed(int x, int y, int button) {
	targetColor = cam.getPixelsRef().getColor(x, y);
	contourFinder.setTargetColor(targetColor, TRACK_COLOR_HSV);
}

void testApp::keyPressed(int key) {
}