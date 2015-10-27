#include "ofApp.h"

void ofApp::setup() {
    useGaussian = false;
    cam.setup(640, 480);
}

void ofApp::update() {
    cam.update();
    if(cam.isFrameNew()) {
        ofxCv::copy(cam, img);
        if(useGaussian) {
            ofxCv::GaussianBlur(img, 50);
        } else {
            ofxCv::blur(img, 50);
        }
        img.update();
    }
}

void ofApp::draw() {
    if(img.isAllocated()) {
        img.draw(0, 0);
    }
    ofDrawBitmapStringHighlight(useGaussian ? "GaussianBlur()" : "blur()", 10, 20);
}

void ofApp::keyPressed(int key) {
    useGaussian = !useGaussian;
}