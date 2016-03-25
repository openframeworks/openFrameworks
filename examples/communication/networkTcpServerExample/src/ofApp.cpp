#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

	ofSetBackgroundColor(20);

	// setup the server to listen on 11999
	ofxTCPSettings settings(11999);

	// set other options
	//settings.blocking = false;
	//settings.messageDelimiter = "\n";

	TCP.setup(settings);

	// optionally set the delimiter to something else.  The delimiter in the client and the server have to be the same, default being [/TCP]
	TCP.setMessageDelimiter("\n");
	lastSent = 0;

}

//--------------------------------------------------------------
void ofApp::update(){

	// for each client lets send them a message letting them know what port they are connected on
	// we throttle the message sending frequency to once every 100ms
	uint64_t now = ofGetElapsedTimeMillis();
	if(now - lastSent >= 100){
		for(int i = 0; i < TCP.getLastID(); i++){
			if( !TCP.isClientConnected(i) ) continue;

			TCP.send(i, "hello client - you are connected on port - "+ofToString(TCP.getClientPort(i)) );
		}
		lastSent = now;
	}

}

//--------------------------------------------------------------
void ofApp::draw(){

	ofSetColor(220);
	ofDrawBitmapString("TCP SERVER Example \n\nconnect on port: "+ofToString(TCP.getPort()), 10, 20);

	ofSetColor(0);
	ofDrawRectangle(10, 60, ofGetWidth()-24, ofGetHeight() - 65 - 15);

	ofSetColor(220);

	// for each connected client lets get the data being sent and lets print it to the screen
	for(unsigned int i = 0; i < (unsigned int)TCP.getLastID(); i++){

		if( !TCP.isClientConnected(i) )continue;

		// give each client its own color
		ofSetColor(255 - i*30, 255 - i * 20, 100 + i*40);

		// calculate where to draw the text
		int xPos = 15;
		int yPos = 80 + (12 * i * 4);

		// get the ip and port of the client
		string port = ofToString( TCP.getClientPort(i) );
		string ip   = TCP.getClientIP(i);
		string info = "client "+ofToString(i)+" -connected from "+ip+" on port: "+port;


		// if we don't have a string allocated yet
		// lets create one
		if(i >= storeText.size() ){
			storeText.push_back( string() );
		}

		// receive all the available messages, separated by \n
		// and keep only the last one
		string str;
		string tmp;
		do{
			str = tmp;
			tmp = TCP.receive(i);
		}while(tmp!="");

		// if there was a message set it to the corresponding client
		if(str.length() > 0){
			storeText[i] = str;
		}

		// draw the info text and the received text bellow it
		ofDrawBitmapString(info, xPos, yPos);
		ofDrawBitmapString(storeText[i], 25, yPos + 20);

	}

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
