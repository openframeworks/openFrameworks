#include "ofApp.h"

#define RECONNECT_TIME 400

//--------------------------------------------------------------
void ofApp::setup(){
	// we don't want to be running to fast
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofEnableAntiAliasing();

	//create the socket and set to send to 127.0.0.1:11999
	ofxUDPSettings settings;
	settings.sendTo("127.0.0.1", 11999);
	settings.blocking = false;

	udpConnection.Setup(settings);

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

	for(unsigned int i=1;i<stroke.size();i++){
		ofDrawLine(stroke[i-1].x,stroke[i-1].y,stroke[i].x,stroke[i].y);
	}

	string buf = "openFrameworks UDP Send Example\n";
	buf += "drag to draw";
	ofDrawBitmapStringHighlight(buf, 15, 30);

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
	stroke.push_back(ofPoint(x,y));
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	stroke.clear();
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	string message="";
	for(unsigned int i=0; i<stroke.size(); i++){
		message+=ofToString(stroke[i].x)+"|"+ofToString(stroke[i].y)+"[/p]";
	}
	udpConnection.Send(message.c_str(),message.length());
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
