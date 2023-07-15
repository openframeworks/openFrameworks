#include "ofApp.h"

void ofApp::setup() {}

void ofApp::update() {
	while(auto m = osc_receiver.getMessage()){
		ofLogNotice("got message") << m->getAddress() << " = " << m->getArgAsInt(0) << " @ " << m->getArgAsFloat(1);
	}
}

void ofApp::draw() {
	ofDrawBitmapString("press space and watch the console", 20, 20);
}

void ofApp::keyPressed(int key) {
	if (key == ' ') {
		ofxOscMessage m { "/message" };
		osc_sender.send(m.addIntArg(ofRandom(100)).addFloatArg(ofGetElapsedTimef()));
	}
}

/* boilerplate
 
void ofApp::keyReleased(int key){}
void ofApp::mouseMoved(int x, int y){}
void ofApp::mouseDragged(int x, int y, int button){}
void ofApp::mousePressed(int x, int y, int button){}
void ofApp::mouseReleased(int x, int y, int button){}
void ofApp::mouseEntered(int x, int y){}
void ofApp::mouseExited(int x, int y){}
void ofApp::windowResized(int w, int h){}
void ofApp::gotMessage(ofMessage msg){}
void ofApp::dragEvent(ofDragInfo dragInfo){}
 
*/
