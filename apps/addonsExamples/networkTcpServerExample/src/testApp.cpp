#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	//we run at 60 fps!
	ofSetVerticalSync(true);

	//load our type
	mono.loadFont("type/mono.ttf", 9);
	monosm.loadFont("type/mono.ttf", 8);

	//setup the server to listen on 11999
	TCP.setup(11999);
}

//--------------------------------------------------------------
void testApp::update(){
	ofBackground(20, 20, 20);

	//for each client lets send them a message letting them know what port they are connected on
	for(int i = 0; i < TCP.getNumClients(); i++){
		TCP.send(i, "hello client - you are connected on port - "+ofToString(TCP.getClientPort(i)) );
	}

}

//--------------------------------------------------------------
void testApp::draw(){

	ofSetHexColor(0xDDDDDD);
	mono.drawString("TCP SERVER Example \n\nconnect on port: "+ofToString(TCP.getPort()), 10, 20);

	ofSetHexColor(0x000000);
	ofRect(10, 60, ofGetWidth()-24, ofGetHeight() - 65 - 15);

	ofSetHexColor(0xDDDDDD);

	//for each connected client lets get the data being sent and lets print it to the screen
	for(int i = 0; i < TCP.getNumClients(); i++){

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
		monosm.drawString(info, xPos, yPos);
		monosm.drawString(storeText[i], 25, yPos + 20);

	}

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
}


//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

