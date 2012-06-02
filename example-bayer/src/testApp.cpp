#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	ofSetVerticalSync(true);

	bayer.loadImage("bayer.png");
	rgb.loadImage("rgb.png");
	
	bayerType = 0;
}

void testApp::update() {
	int code;
	switch(bayerType) {
		case 0: code = CV_BayerBG2RGB; break;
		case 1: code = CV_BayerGB2RGB; break;
		case 2: code = CV_BayerRG2RGB; break;
		case 3: code = CV_BayerGR2RGB; break;
	}
	
	convertColor(bayer, rgb, code);
	rgb.update();
}

void testApp::draw() {
	ofPushMatrix();
	bayer.draw(0, 0);
	ofTranslate(rgb.getWidth(), 0);
	rgb.draw(0, 0);
	ofTranslate(bayer.getWidth(), 0);
	ofPopMatrix();
	
	drawHighlightString("use the up/down keys: " + ofToString(bayerType), 10, 10);
}

void testApp::keyPressed(int key) {
	if(key == OF_KEY_UP) {
		bayerType--;
	}
	if(key == OF_KEY_DOWN) {
		bayerType++;
	}
	bayerType = ofClamp(bayerType, 0, 3);
}