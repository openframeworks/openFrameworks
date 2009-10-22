#include "testApp.h"

#define RECONNECT_TIME 400

//--------------------------------------------------------------
void testApp::setup(){

	// we don't want to be running to fast
	ofSetVerticalSync(true);
	ofSetFrameRate(60);

	//load our type
	mono.loadFont("type/mono.ttf",9);
	monosm.loadFont("type/mono.ttf",8);

    //create the socket and set to send to 127.0.0.1:11999
	udpConnection.Create();
	udpConnection.Connect("127.0.0.1",11999);
	udpConnection.SetNonBlocking(true);


}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

	ofSetColor(20, 20, 20);
	mono.drawString("openFrameworks UDP Send Example ", 15, 30);
    monosm.drawString("drag to draw", 15, 50);
	for(int i=1;i<stroke.size();i++){
		ofLine(stroke[i-1].x,stroke[i-1].y,stroke[i].x,stroke[i].y);
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
	stroke.push_back(ofPoint(x,y));
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	stroke.clear();
}


//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	string message="";
	for(int i=0; i<stroke.size(); i++){
		message+=ofToString(stroke[i].x)+"|"+ofToString(stroke[i].y)+"[/p]";
	}
	int sent = udpConnection.Send(message.c_str(),message.length());
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
}
