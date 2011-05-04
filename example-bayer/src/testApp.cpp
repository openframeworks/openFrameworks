#include "testApp.h"

void testApp::setup() {
	ofSetVerticalSync(true);

	bayer.loadImage("bayer.png");
	rgb.loadImage("rgb.png");
	yuv.loadImage("yuv.png");
	
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
	
	Mat bayerMat = toCv(bayer);
	Mat rgbMat = toCv(rgb);
	cvtColor(bayerMat, rgbMat, code);
	rgb.update();
}

void testApp::draw() {
	rgb.draw(0, 0);
	ofTranslate(rgb.getWidth(), 0);
	bayer.draw(0, 0);
	ofTranslate(bayer.getWidth(), 0);
	yuv.draw(0, 0);
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