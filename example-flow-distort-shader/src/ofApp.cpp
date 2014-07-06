#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
	cam.initGrabber(1280, 720);
    motionAmplifier.setup(cam.getWidth(), cam.getHeight(), 2, .25);
}

void ofApp::update() {
    motionAmplifier.setStrength(ofMap(mouseX, 0, ofGetWidth(), -10, 10));
    motionAmplifier.setLearningRate(ofMap(mouseY, 0, ofGetHeight(), 0, 1, true));
    motionAmplifier.setBlurAmount(0);
    motionAmplifier.setWindowSize(8);
    
	cam.update();
	if(cam.isFrameNew()) {
        motionAmplifier.update(cam);
	}
}

void ofApp::draw() {
    ofBackground(0);
    ofSetupScreenOrtho(ofGetWidth(), ofGetHeight(), -100, +100);
    ofEnableDepthTest();
    motionAmplifier.draw(cam);
//    motionAmplifier.drawMesh();
    ofDisableDepthTest();
}