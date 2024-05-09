#include "ofApp.h"

void ofApp::setup(){
	g1.setRect(ofRectangle(0, 0, ofGetWindowWidth() * .5, ofGetWindowHeight()) );
	g2.setRect(ofRectangle(ofGetWindowWidth() * .5, 0, ofGetWindowWidth() * .5, ofGetWindowHeight()) );
	g2.color = ofColor(0, 255, 0);
	g1.name = "OF fps count drift";
	g2.name = "new fps count drift";
	fpsToggle.setup();
}

void ofApp::update() {
}

void ofApp::draw(){
	
	float ratio = (float)ofGetMouseX() / ofGetWindowWidth();
	count.valRatio = ratio;
	
	ofSetColor(0, 0, 255, 3);
	float fpsNeue = count.get();
	ofDrawRectangle(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
	float driftOF = fpsToggle.fps - ofGetFrameRate();
	float drift = fpsToggle.fps - fpsNeue;
	
	if (ofGetFrameNum() > 5) {
		g1.add(driftOF);
		g2.add(drift);
	}
	g1.draw();
	g2.draw();

	string t = 	"of fRate: " + ofToString(ofGetFrameRate()) + "\n" +
//				"of fps difference: " + ofToString(driftOF) + "\n" +
				"new fRate: " + ofToString(fpsNeue) + " \n" +
//				"new fps difference: " + ofToString(drift) + "\n" +
	"new fps: " + ofToString(fpsToggle.isNew) + "\n" +

				""
//				"vsync:" + ofToString(vs) + "\n"
	;

	ofSetColor(255);
	ofDrawBitmapString(t, 10, ofGetWindowHeight() - 85);
	
	fpsToggle.update();
	count.tick();
}

void ofApp::keyPressed(int key) {
	count.reset();

	if (key == '1') {
		fpsToggle.setFps(30);
	}
	if (key == '2') {
		fpsToggle.setFps(60);
	}
	if (key == '3') {
		fpsToggle.setFps(120);
	}
	if (key == '4') {
		fpsToggle.setFps(20);
	}
	if (key == '5') {
		fpsToggle.setFps(15);
	}
	if (key == 't') {
		fpsToggle.toggle();
		g1.reset();
		g2.reset();
//		count.reset();
	}
	
	if (key == 'r') {
		ofSetFrameRate(ofRandom(25, 100));
	}

	if (key == 'f') {
		ofToggleFullscreen();
	}
}
