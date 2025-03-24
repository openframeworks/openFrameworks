#include "ofApp.h"

void ofApp::update() {

	// you can check the state of a specific ofParameter
	// (wrapper around it's own "hidden Event")
	// no listeners/callbacks are required

	if (size.didNotify()) {
		ofLogNotice("new size") << size;
	}

	if (refresh.didNotify()) {
		color = ofColor::fromHsb(ofRandom(360), 128, 128);
		ofLogNotice("boosch") << color;
	}

	if (animate.didNotify()) { // this is the ofAbstractParameter::did_notify()
		if (animate != animating) { // sanity check
			animating = animate;
			if (animating) {
				ofLogNotice("stop animation");
			} else {
				ofLogNotice("begin animation");
				size = 0;
			}
		}
	}

	if (auto what = params.parameterChangedE().didNotify()) { // this is the Events::did_notify()
		// something happended in params, but you don't know what
	}
}

void ofApp::draw() {
	ofBackground(0);
	if (animating) {
		ofSetColor(color);
		ofDrawCircle(ofGetWidth() / 2, ofGetHeight() / 2, size);
		size = size + 1;
		if (size > 100) size -= 100;
	}
	ofDrawBitmapString("<a> to toggle automation", 10, 10);
	ofDrawBitmapString("<r> to randomize size", 10, 20);
	ofDrawBitmapString("<c> to change color", 10, 30);
}

void ofApp::keyPressed(int key) {
	if (key == 'a') animate = !animate;
	if (key == 'r') size = ofRandom(50, 100);
	if (key == 'c') refresh.trigger();
}
