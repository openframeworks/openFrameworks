#include "testApp.h"

void testApp::setup() {	
	quality = OF_IMAGE_QUALITY_LOW;
	maxSize = 2048;
	reset();
}

void testApp::reset() {
	generation = 0;
	img.loadImage("buses.jpg");
	addX = ofRandom(1, 16);
	addY = ofRandom(1, 16);
	subX = ofRandom(1, addX);
	subY = ofRandom(1, addY);
}

void testApp::update() {
	string curFilename = "compressed.jpg";
	
	int size = img.getWidth();
	
	if(size < maxSize) {
		if(ofGetFrameNum() % 2 == 0) {
			img.saveImage(curFilename, quality);
			img.loadImage(curFilename);			
			img.resize(size + addX, size + addY);
		} else {
			img.saveImage(curFilename, quality);
			img.loadImage(curFilename);
			img.resize(size - subX, size - subY);
		}
		generation++;
	}
}

void testApp::draw() {
	ofSetColor(255);
	img.draw(0, 0, ofGetWidth(), ofGetHeight());
	
	ofSetColor(0);
	ofRect(5, 5, 250, 45);
	ofSetColor(255);
	ofDrawBitmapString("Currently on generation " + ofToString(generation), 10, 20);
	stringstream out;
	out << addX << " " << addY << " " << subX << " " << subY;
	ofDrawBitmapString(out.str(), 10, 40);
}

void testApp::mousePressed(int x, int y, int button) {
	reset();
}
