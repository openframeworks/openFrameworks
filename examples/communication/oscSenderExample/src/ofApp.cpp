#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	// open an outgoing connection to HOST:PORT
	sender.setup(HOST, PORT);
	imgAsBuffer = ofBufferFromFile("of-logo.png", true);

	ofSetWindowTitle("osc sender");

}

//--------------------------------------------------------------
void ofApp::update(){
	ofxOscMessage m;
	m.setAddress("/mouse/position");
	m.addFloatArg(ofMap(ofGetMouseX(), 0, ofGetWidth(), 0.f, 1.f, true));
	m.addFloatArg(ofMap(ofGetMouseY(), 0, ofGetHeight(), 0.f, 1.f, true));
	sender.sendMessage(m, false);
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(255, 100);

	if(img.getWidth() > 0){
		ofSetColor(255);
		img.draw(ofGetWidth()/2-img.getWidth()/2,
			ofGetHeight()/2-img.getHeight()/2);
	}

	// display instructions
	string buf = "sending osc messages to : " + string(HOST);
	buf += " : " + ofToString(PORT);
	buf += "\npress A to send osc message [/test 1 3.5 hello <time>]\n";
	buf += "press I to send a (small) image as a osc blob to [/image]";
	ofDrawBitmapStringHighlight(buf, 10, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == 'a' || key == 'A'){
		ofxOscMessage m;
		m.setAddress("/test");
		m.addIntArg(1);
		m.addFloatArg(3.5f);
		m.addStringArg("hello");
		m.addFloatArg(ofGetElapsedTimef());
		sender.sendMessage(m, false);
	}

	//send an image. (Note: the size of the image depends greatly on your network buffer sizes - if an image is too big the message won't come through )

	if( key == 'i' || key == 'I'){
		img.load(imgAsBuffer);

		ofxOscMessage m;
		m.setAddress("/image");
		m.addBlobArg(imgAsBuffer);
		sender.sendMessage(m);
		cout << "ofApp:: sending image with size: " << imgAsBuffer.size() << endl;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	ofxOscMessage m;
	m.setAddress("/mouse/button");
	m.addIntArg(1);
	m.addStringArg("down");
	sender.sendMessage(m, false);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	ofxOscMessage m;
	m.setAddress("/mouse/button");
	m.addIntArg(0);
	m.addStringArg("up");
	sender.sendMessage(m, false);
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}

