#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
	ofSetVerticalSync(true);
	
	kalman.init(1e-4, 1e-1); // invert of (smoothness, rapidness)
	
	line.setMode(OF_PRIMITIVE_LINE_STRIP);
	predicted.setMode(OF_PRIMITIVE_LINE_STRIP);
	estimated.setMode(OF_PRIMITIVE_LINE_STRIP);
	
	speed = 0.f;
}

void ofApp::update() {
	ofVec2f curPoint(mouseX, mouseY);
	line.addVertex(curPoint);
	
	kalman.update(curPoint); // feed measurement
	
	point = kalman.getPrediction(); // prediction before measurement
	predicted.addVertex(point);
	estimated.addVertex(kalman.getEstimation()); // corrected estimation after measurement
	
	speed = kalman.getVelocity().length();
	int alpha = ofMap(speed, 0, 20, 50, 255, true);
	line.addColor(ofColor(255, 255, 255, alpha));
	predicted.addColor(ofColor(255, 0, 0, alpha));
	estimated.addColor(ofColor(0, 255, 0, alpha));
}

void ofApp::draw() {
	ofBackground(0);
	
	line.draw();
	
	predicted.draw();
	ofPushStyle();
	ofSetColor(ofColor::red, 128);
	ofFill();
	ofCircle(point, speed * 2);
	ofPopStyle();
	
	estimated.draw();
}
