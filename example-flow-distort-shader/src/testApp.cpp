#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	cam.initGrabber(1280, 720);
    motionAmplifier.setup(cam.getWidth(), cam.getHeight(), 2, .25);
}

void testApp::update() {
    motionAmplifier.setStrength(ofMap(mouseX, 0, ofGetWidth(), -10, 10));
    motionAmplifier.setLearningRate(ofMap(mouseY, 0, ofGetHeight(), 0, 1, true));
    motionAmplifier.setBlurAmount(0);
    motionAmplifier.setWindowSize(8);
    
	cam.update();
	if(cam.isFrameNew()) {
        motionAmplifier.update(cam);
	}
}

void testApp::draw() {
    ofBackground(0);
    ofSetupScreenOrtho(ofGetWidth(), ofGetHeight(), -100, +100);
    ofEnableDepthTest();
    motionAmplifier.draw(cam);
//    motionAmplifier.drawMesh();
    ofDisableDepthTest();
}