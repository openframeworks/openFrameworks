#include "ofApp.h"
#include "ofxAndroidVideoGrabber.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(255,255,255);
	ofSetLogLevel(OF_LOG_NOTICE);
	ofSetOrientation(OF_ORIENTATION_90_LEFT);
	grabber.initGrabber(640,480);
	ofSetFrameRate(60);
	one_second_time = ofGetElapsedTimeMillis();
	camera_fps = 0;
	frames_one_sec = 0;
	
	ofx::HTTP::BasicIPVideoServerSettings settings;

    // Many other settings are available.
    settings.setPort(7890);

    // The default maximum number of client connections is 5.
    settings.setMaxClientConnections(1);

    // Apply the settings.
    server = ofx::HTTP::BasicIPVideoServer::makeShared(settings);

    // Start the server.
    server->start();
	
}

//--------------------------------------------------------------
void ofApp::update(){
	grabber.update();
	if(grabber.isFrameNew()){
		frames_one_sec++;
		if( ofGetElapsedTimeMillis() - one_second_time >= 1000){
			camera_fps = frames_one_sec;
			frames_one_sec = 0;
			one_second_time = ofGetElapsedTimeMillis();
		}
		 server->send(grabber.getPixelsRef());
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetHexColor(0xFFFFFF);
	grabber.draw(20,20);
	ofSetHexColor(0x000000);
	std::stringstream ss;

    ss << "Num clients connected: ";
    ss << server->getNumConnections();

    ofDrawBitmapStringHighlight(ss.str(), 20, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){ 
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::touchDown(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchMoved(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchUp(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchCancelled(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::swipe(ofxAndroidSwipeDir swipeDir, int id){

}

//--------------------------------------------------------------
void ofApp::pause(){

}

//--------------------------------------------------------------
void ofApp::stop(){

}

//--------------------------------------------------------------
void ofApp::resume(){

}

//--------------------------------------------------------------
void ofApp::reloadTextures(){

}

//--------------------------------------------------------------
bool ofApp::backPressed(){
	return false;
}

//--------------------------------------------------------------
void ofApp::okPressed(){

}

//--------------------------------------------------------------
void ofApp::cancelPressed(){

}
