#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(255,255,255);
	ofSetLogLevel(OF_LOG_NOTICE);
	ofSetOrientation(OF_ORIENTATION_90_LEFT);
	// this makes the camera directly return grayscale image, faster!
	grabber.setPixelFormat(OF_PIXELS_MONO);
	grabber.setup(1280, 720);
	gray.allocate((int) grabber.getWidth(), (int) grabber.getHeight());
	bg.allocate((int) grabber.getWidth(), (int) grabber.getHeight());
	one_second_time =0;
	camera_fps = 0;
	frames_one_sec = 0;
	captureBg = true;
}

//--------------------------------------------------------------
void ofApp::update(){
	grabber.update();
	if(grabber.isFrameNew()){
		frames_one_sec++;
		if(ofGetElapsedTimef() - one_second_time >= 1.0){
			camera_fps = frames_one_sec;
			frames_one_sec = 0;
			one_second_time = ofGetElapsedTimef();
		}
		gray.setFromPixels(grabber.getPixels());
		if(captureBg){
			bg = gray;
			captureBg = false;
		}

		gray.threshold(80);
		contourFinder.findContours(gray,grabber.getWidth()*grabber.getHeight()/20,grabber.getWidth()*grabber.getHeight()/3,10,true,true);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(255, 255, 255);
	gray.draw(0,0, 1920, 1080);
	contourFinder.draw(0,0, 1920, 1080);
	ofSetColor(255, 0, 255);
	ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate()),100,10);
	ofDrawBitmapString("camera fps: " + ofToString(camera_fps),100,30);
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