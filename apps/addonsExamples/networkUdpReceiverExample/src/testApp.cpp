#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	//we run at 60 fps!
	ofSetVerticalSync(true);
	ofSetFrameRate(60);

	//load our type
	mono.loadFont("type/mono.ttf", 9);
	monosm.loadFont("type/mono.ttf", 8);

    //create the socket and bind to port 11999
	udpConnection.Create();
	udpConnection.Bind(11999);
	udpConnection.SetNonBlocking(true);

	ofBackground(255, 255, 255);
	ofSetBackgroundAuto(false);
}

//--------------------------------------------------------------
void testApp::update(){

	char udpMessage[100000];
	udpConnection.Receive(udpMessage,100000);
	string message=udpMessage;
	if(message!=""){
		stroke.clear();
		float x,y;
		vector<string> strPoints = ofSplitString(message,"[/p]");
		for(int i=0;i<strPoints.size();i++){
			vector<string> point = ofSplitString(strPoints[i],"|");
			x=atof(point[0].c_str());
			y=atof(point[1].c_str());
			point.clear();
			stroke.push_back(ofPoint(x,y));
		}
	}

}
//--------------------------------------------------------------
void testApp::draw(){
    ofFill();
    ofSetHexColor(0xFFFFFF);
    ofRect(0,0,200,30);
	ofSetHexColor(0x101010);
	mono.drawString("UDP Receiver Example ", 10, 20);

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
