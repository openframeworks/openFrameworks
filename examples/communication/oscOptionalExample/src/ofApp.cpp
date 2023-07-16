#include "ofApp.h"

void ofApp::setup() {
	// look Ma, no hands!
}

void ofApp::update() {
	// ofxOscMessage alloc is out of sight
	while (const auto m = osc_receiver.getMessage()) {
		// valid optional is accessed like a pointer
		ofLogNotice(m->getAddress()) << m->getArgTypeName(0) << ": " << m->getArgAsInt(0) << " @ " << m->getArgTypeName(1) << ": " << m->getArgAsFloat(1);
	}
}

void ofApp::draw() {
	ofDrawBitmapString("press space and watch the console", 20, 20);
}

void ofApp::keyPressed(int key) {
	if (key == ' ') {
		
		// old-timer
		ofxOscMessage m;
		m.setAddress("/message");
		m.addIntArg(ofRandom(100));
		m.addFloatArg(ofGetElapsedTimef());
		osc_sender.sendMessage(m);

		// ctor, chaining, overloaded operator+ (implicit types), in-place overloaded send
		osc_sender.send(ofxOscMessage{"/terse"} + int(ofRandom(100)) + ofGetElapsedTimef());
		
		// of course it's a spectrum, this is familiar yet fresh:
		ofxOscMessage m2 { "/comfortable" };
		m2.add(int(ofRandom(100)));
		m2.add(ofGetElapsedTimef());
		osc_sender.send(m2);
		
	}
}

//void ofApp::keyReleased(int key){}
//void ofApp::mouseMoved(int x, int y){}
//void ofApp::mouseDragged(int x, int y, int button){}
//void ofApp::mousePressed(int x, int y, int button){}
//void ofApp::mouseReleased(int x, int y, int button){}
//void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY) {};
//void ofApp::mouseEntered(int x, int y){}
//void ofApp::mouseExited(int x, int y){}
//void ofApp::windowResized(int w, int h){}
//void ofApp::gotMessage(ofMessage msg){}
//void ofApp::dragEvent(ofDragInfo dragInfo){}
