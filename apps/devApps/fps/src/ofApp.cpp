#include "ofApp.h"

void ofApp::setup(){
	g1.setRect(ofRectangle(0, 0, ofGetWindowWidth() * .5, ofGetWindowHeight()) );
	g2.setRect(ofRectangle(ofGetWindowWidth() * .5, 0, ofGetWindowWidth() * .5, ofGetWindowHeight()) );
	g2.color = ofColor(0, 255, 0);
	g1.name = "OF drift in milliFrames";
	g2.name = "new drift in milliFrames";
	fpsToggle.setup();
}

void ofApp::update() {
}

void ofApp::draw(){
	count.tick();

//	float ratio = (float)ofGetMouseX() / ofGetWindowWidth();
//	count.valRatio = ratio;
	
	ofSetColor(0, 0, 255, 3);
	float fpsNeue = count.get();
	ofDrawRectangle(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
	float driftOF = fpsToggle.fps - ofGetFrameRate();
	float drift = fpsToggle.fps - fpsNeue;
	
	if (ofGetFrameNum() > 5) {
		g1.add(driftOF * 1000.0f);
		g2.add(drift * 1000.0f);
	}
	g1.draw();
	g2.draw();

	string t =
				"set fps: " + ofToString(fpsToggle.fps) + "\n" +
				"OF fRate: " + ofToString(ofGetFrameRate()) + "\n" +
//				"of fps difference: " + ofToString(driftOF) + "\n" +
				"new fRate: " + ofToString(fpsNeue) + " \n\n" +
//				"new fps difference: " + ofToString(drift) + "\n" +
				(fpsToggle.isNew ? "OF New Framerate Control" : "OF Classic Framerate Control") + "\n" +
				"press T to toggle classic/new"
	// "new fps: " + ofToString(fpsToggle.isNew) + "\n" +

				""
//				"vsync:" + ofToString(vs) + "\n"
	;

	ofSetColor(255);
	ofDrawBitmapString(t, 10, ofGetWindowHeight() - 85);
	
	fpsToggle.update();
	
}

void ofApp::keyPressed(int key) {

	if (key == '1') {
		fpsToggle.setFps(30);
		count.reset();

	}
	if (key == '2') {
		fpsToggle.setFps(60);
		count.reset();
	}
	if (key == '3') {
		fpsToggle.setFps(120);
		count.reset();
	}
	if (key == '4') {
		fpsToggle.setFps(20);
		count.reset();
	}
	if (key == '5') {
		fpsToggle.setFps(15);
		count.reset();
	}
	if (key == 't') {
		fpsToggle.toggle();
		count.reset();
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
