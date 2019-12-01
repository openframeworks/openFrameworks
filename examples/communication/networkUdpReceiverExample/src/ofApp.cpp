#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//we run at 60 fps!
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofEnableAntiAliasing();

	//create the socket and bind to port 11999
	ofxUDPSettings settings;
	settings.receiveOn(11999);
	settings.blocking = false;

	udpConnection.Setup(settings);

	ofSetBackgroundAuto(false);
	ofSetBackgroundColor(255);
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
				stroke.push_back(glm::vec3(x,y,0));
			}
		}
	}

}

//--------------------------------------------------------------
void ofApp::draw(){

	ofSetColor(20);
	for(unsigned int i=1;i<stroke.size();i++){
		ofDrawLine(stroke[i-1].x,stroke[i-1].y,stroke[i].x,stroke[i].y);
	}

	ofDrawBitmapStringHighlight("UDP Receiver Example ", 10, 20);
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
