#include "ofApp.h"

void ofApp::setup() {
	// Look Ma, No Hands!
}

void ofApp::update() {
	while (const auto m = osc_receiver.getMessage()) {
		ofLogNotice(m->getAddress()) << m->getArgAsInt(0) << " @ " << m->getArgAsFloat(1);
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

		// ctor + chaining (implicit types) + polymorphic send
		osc_sender.send(ofxOscMessage{"/terse"}.add(ofRandom(100)).add(ofGetElapsedTimef()));
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
