#include "ofApp.h"

#define RECONNECT_TIME 400

//--------------------------------------------------------------
void ofApp::setup(){

	// we don't want to be running to fast
	ofSetVerticalSync(true);

	//some variables:
	//have we typed
	typed	= false;

	//our typing position
	pos		= 0;

	//our send and recieve strings
	msgTx	= "";
	msgRx	= "";

	//are we connected to the server - if this fails we
	//will check every few seconds to see if the server exists
	weConnected = tcpClient.setup("127.0.0.1", 11999);
	//optionally set the delimiter to something else.  The delimter in the client and the server have to be the same
	tcpClient.setMessageDelimiter("\n");
	
	connectTime = 0;
	deltaTime = 0;

	tcpClient.setVerbose(true);

}

//--------------------------------------------------------------
void ofApp::update(){
	ofBackground(230, 230, 230);

	//we are connected - lets send our text and check what we get back
	if(weConnected){
		if(tcpClient.send(msgTx)){

			//if data has been sent lets update our text
			string str = tcpClient.receive();
			if( str.length() > 0 ){
				msgRx = str;
			}
		}else if(!tcpClient.isConnected()){
			weConnected = false;
		}
	}else{
		//if we are not connected lets try and reconnect every 5 seconds
		deltaTime = ofGetElapsedTimeMillis() - connectTime;

		if( deltaTime > 5000 ){
			weConnected = tcpClient.setup("127.0.0.1", 11999);
			connectTime = ofGetElapsedTimeMillis();
		}

	}
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofSetColor(20, 20, 20);
	ofDrawBitmapString("openFrameworks TCP Send Example", 15, 30);

	if(typed){
		ofDrawBitmapString("sending:", 15, 55);
		ofDrawBitmapString(msgTx, 85, 55);
	}
	else{
		if(weConnected)ofDrawBitmapString("status: type something to send data to port 11999", 15, 55);
		else ofDrawBitmapString("status: server not found. launch server app and check ports!\n\nreconnecting in "+ofToString( (5000 - deltaTime) / 1000 )+" seconds", 15, 55);
	}

	ofDrawBitmapString("from server: \n"+msgRx, 15, 270);

}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	//you can only type if you're connected
	if(weConnected){
		if(key == 13)key = '\n';
		if(key == 8 || key == 127){
			if( pos != 0 ){pos--;
				msgTx = msgTx.substr(0,pos);
			}else msgTx = "";
		}else{
			msgTx.append(1, (char) key);
			pos++;
		}
		typed = true;
	}
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
