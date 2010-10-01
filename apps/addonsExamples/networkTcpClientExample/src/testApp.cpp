#include "testApp.h"

#define RECONNECT_TIME 400

//--------------------------------------------------------------
void testApp::setup(){

	// we don't want to be running to fast
	ofSetVerticalSync(true);

	//load our type
	mono.loadFont("type/mono.ttf",9);
	monosm.loadFont("type/mono.ttf",8);

	//some variables

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

	connectTime = 0;
	deltaTime = 0;

	tcpClient.setVerbose(true);

}

//--------------------------------------------------------------
void testApp::update(){
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
void testApp::draw(){

	ofSetColor(20, 20, 20);
	mono.drawString("openFrameworks TCP Send Example", 15, 30);

	if(typed){
		monosm.drawString("sending:", 15, 55);
		monosm.drawString(msgTx, 85, 55);
	}
	else{
		if(weConnected)monosm.drawString("status: type something to send data to port 11999", 15, 55);
		else monosm.drawString("status: server not found. launch server app and check ports!\n\nreconnecting in "+ofToString( (5000 - deltaTime) / 1000 )+" seconds", 15, 55);
	}

	monosm.drawString("from server: \n"+msgRx, 15, 270);

}


//--------------------------------------------------------------
void testApp::keyPressed(int key){

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

