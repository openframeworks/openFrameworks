#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//we run at 60 fps!
	ofSetVerticalSync(true);
	ofSetFrameRate(60);

    //create the socket and bind to port 11999
	udpConnection.Create();
	udpConnection.Bind(11999);
	udpConnection.SetNonBlocking(true);

	ofSetBackgroundAuto(false);
	ofBackground(255, 255, 255);
}

//--------------------------------------------------------------
void ofApp::update(){

	char udpMessage[100000];
	udpConnection.Receive(udpMessage,100000);
	string message=udpMessage;
	if(message!=""){
		stroke.clear();
		float x,y;
		vector<string> strPoints = ofSplitString(message,"[/p]");
		for(unsigned int i=0;i<strPoints.size();i++){
			vector<string> point = ofSplitString(strPoints[i],"|");
			if( point.size() == 2 ){
				x=atof(point[0].c_str());
				y=atof(point[1].c_str());
				stroke.push_back(ofPoint(x,y));
			}
		}
	}

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofFill();
    ofSetHexColor(0xFFFFFF);
    ofRect(0,0,200,30);
	ofSetHexColor(0x101010);
	ofDrawBitmapString("UDP Receiver Example ", 10, 20);

	for(unsigned int i=1;i<stroke.size();i++){
		ofLine(stroke[i-1].x,stroke[i-1].y,stroke[i].x,stroke[i].y);
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
