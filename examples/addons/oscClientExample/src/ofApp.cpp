#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofBackground(40, 100, 40);
	ofSetVerticalSync(true);
	ofSetFrameRate(60);

	// open an outgoing connection to SERVER_HOST:SERVER_PORT
	sender.setup(SERVER_HOST, SERVER_PORT);

	// set up a listener on the sender's local port,
	// this way the server can respond directly to the
	// address from which it receives our messages
	localPort = sender.getLocalPort();
	cout << "listening for OSC messages on port " << localPort << "\n";
	receiver.setup(localPort);
}

//--------------------------------------------------------------
void ofApp::update(){

	log.update();

	while(receiver.hasWaitingMessages()){
		ofxOscMessage m;
		receiver.getNextMessage(m);

		if("/calculate/sum" == m.getAddress()){
			// the sum is an int32
			int sum = m.getArgAsInt32(0);
			string msg_string;
			msg_string = "[" + ofToString(SERVER_PORT) +"] " + m.getAddress();
			msg_string += ": ";
			msg_string += ofToString(sum);
			log.append(msg_string, 5.0f);
		}
		else if("/error" == m.getAddress()){
			string msg_string;
			msg_string = "[" + ofToString(SERVER_PORT) +"] ";
			msg_string += m.getArgAsString(0);
			log.append(msg_string, 5.0f);
		}
		else{
			log.append("Unknown message received: " + m.getAddress(), 5.0f);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

	// display instructions
	string buf;
	buf = "OSC client (local port " + ofToString(localPort) + ") talking to server at " + string(SERVER_HOST) + ":" + ofToString(SERVER_PORT);
	ofDrawBitmapString(buf, 10, 20);
	ofDrawBitmapString("move the mouse to send osc message [/mouse/position <x> <y>]", 10, 50);
	ofDrawBitmapString("click to send osc message [/mouse/button <button> <\"up\"|\"down\">]", 10, 65);
	ofDrawBitmapString("and listen for response", 25, 80);
	ofDrawBitmapString("press 'x' to send an invalid server request", 10, 95);

	log.draw(10, 115);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	if('x' == key){
		// send an unrecognized request to the server
		ofxOscMessage m;
		m.setAddress("/some/request");
		m.addFloatArg(3.14159);
		m.addStringArg("whoops");
		m.addIntArg(-123);
		sender.sendMessage(m, false);
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

	ofxOscMessage m;
	m.setAddress("/mouse/position");
	m.addIntArg(x);
	m.addIntArg(y);
	sender.sendMessage(m, false);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

	ofxOscMessage m;
	m.setAddress("/mouse/button");
	m.addIntArg(button);
	m.addStringArg("down");
	sender.sendMessage(m, false);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

	ofxOscMessage m;
	m.setAddress("/mouse/button");
	m.addIntArg(button);
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
