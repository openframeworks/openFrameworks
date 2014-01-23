#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	//we run at 60 fps!
	ofSetVerticalSync(true);

	//setup the server to listen on 11999
	TCP.setup(11999);
	//optionally set the delimiter to something else.  The delimter in the client and the server have to be the same, default being [/TCP]
	TCP.setMessageDelimiter("\n");
}

//--------------------------------------------------------------
void ofApp::update(){
	ofBackground(20, 20, 20);

	//for each client lets send them a message letting them know what port they are connected on
	for(int i = 0; i < TCP.getLastID(); i++){
		if( !TCP.isClientConnected(i) )continue;
	
		TCP.send(i, "hello client - you are connected on port - "+ofToString(TCP.getClientPort(i)) );
	}

}

//--------------------------------------------------------------
void ofApp::draw(){

	ofSetHexColor(0xDDDDDD);
	ofDrawBitmapString("TCP SERVER Example \n\nconnect on port: "+ofToString(TCP.getPort()), 10, 20);

	ofSetHexColor(0x000000);
	ofRect(10, 60, ofGetWidth()-24, ofGetHeight() - 65 - 15);

	ofSetHexColor(0xDDDDDD);

	//for each connected client lets get the data being sent and lets print it to the screen
	for(unsigned int i = 0; i < (unsigned int)TCP.getLastID(); i++){

		if( !TCP.isClientConnected(i) )continue;

		//give each client its own color
		ofSetColor(255 - i*30, 255 - i * 20, 100 + i*40);

		//calculate where to draw the text
		int xPos = 15;
		int yPos = 80 + (12 * i * 4);

		//get the ip and port of the client
		string port = ofToString( TCP.getClientPort(i) );
		string ip   = TCP.getClientIP(i);
		string info = "client "+ofToString(i)+" -connected from "+ip+" on port: "+port;


		//if we don't have a string allocated yet
		//lets create one
		if(i >= storeText.size() ){
			storeText.push_back( string() );
		}

		//we only want to update the text we have recieved there is data
		string str = TCP.receive(i);

		if(str.length() > 0){
			storeText[i] = str;
		}

		//draw the info text and the received text bellow it
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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
