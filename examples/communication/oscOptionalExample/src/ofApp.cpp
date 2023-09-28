#include "ofApp.h"

void ofApp::setup() {
	// look Ma, no hands !
}

void ofApp::update() {

	// ofxOscMessage alloc is out of sight
	while (const auto m = osc_receiver.getMessage()) {

		// valid optional is accessed like a pointer
		ofLogNotice(std::string("<optional> received ") + m->getAddress())
			<< m->getArgTypeName(0) << ": " << m->getArgAsInt(0) << ", "
			<< m->getArgTypeName(1) << ": " << m->getArgAsFloat(1) << ", "
			<< m->getArgTypeName(2) << ": " << m->getArgAsInt(2);
	}
}

void ofApp::draw() {
	ofDrawBitmapString("press space and watch the console", 20, 20);
}

void ofApp::keyPressed(int key) {
	if (key == ' ') {

		ofLogNotice("sending messages") << ofGetElapsedTimef();

		// old-timer
		ofxOscMessage m;
		m.setAddress("/message");
		m.addIntArg(ofRandom(100));
		m.addFloatArg(ofGetElapsedTimef());
		m.addFloatArg(1982);
		osc_sender.sendMessage(m);

		// ctor, chaining, implicit types, variadic add, in-place, overloaded send
		osc_sender.send(ofxOscMessage { "/terse" }.add(int(ofRandom(100)), ofGetElapsedTimef(), 1983));

		// of course it's a spectrum — this is familiar yet refreshing:
		ofxOscMessage m2 { "/comfortable" };
		m2.add(int(ofRandom(100)), ofGetElapsedTimef(), 1984);
		osc_sender.send(m2);
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
}
