#include "ofApp.h"
#include "ofxAndroidVideoGrabber.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0,0,0);
	ofSetLogLevel(OF_LOG_NOTICE);
	//ofSetOrientation(OF_ORIENTATION_90_LEFT);

	std::shared_ptr<ofxAndroidVideoGrabber> androidGrabber = grabber.getGrabber<ofxAndroidVideoGrabber>();
	androidGrabber->setDeviceID(androidGrabber->getFrontCamera());


	grabber.setup(1280,960);

	one_second_time = ofGetElapsedTimeMillis();
	camera_fps = 0;
	frames_one_sec = 0;

	orientation = androidGrabber->getCameraOrientation();
	frontalFacing = androidGrabber->getIsCameraFacingFront();
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
	}
}

//--------------------------------------------------------------
void ofApp::draw(){    
    // Calculate aspect ratio of grabber image
    float grabberAspectRatio = grabber.getWidth() / grabber.getHeight();

    // Draw camera image centered in the window
    ofSetHexColor(0xFFFFFF);
	ofSetRectMode(OF_RECTMODE_CENTER);

	if(ofGetWidth() > ofGetHeight()) {
		grabber.draw(ofGetWidth() / 2, ofGetHeight() / 2, ofGetHeight() * grabberAspectRatio,
					 ofGetHeight());
	} else {
		grabber.draw(ofGetWidth() / 2, ofGetHeight() / 2, ofGetWidth(),
					 ofGetWidth()  * 1.0/grabberAspectRatio);

	}

	// Draw text gui
	ofSetRectMode(OF_RECTMODE_CORNER);
	ofDrawRectangle(0, 0, 300, 100);
	ofSetHexColor(0x000000);
	ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate()),20,20);
	ofDrawBitmapString("camera fps: " + ofToString(camera_fps),20,40);

	if(frontalFacing) {
		ofDrawBitmapString("facing: front", 20, 60);
	}  else {
		ofDrawBitmapString("facing: back", 20, 60);
	}
	ofDrawBitmapString("orientation: " + ofToString(orientation) ,20,80);
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
	std::shared_ptr<ofxAndroidVideoGrabber> androidGrabber = grabber.getGrabber<ofxAndroidVideoGrabber>();

	if(frontalFacing) {
		androidGrabber->setDeviceID(androidGrabber->getBackCamera());
	} else {
		androidGrabber->setDeviceID(androidGrabber->getFrontCamera());
	}
	orientation = androidGrabber->getCameraOrientation();
	frontalFacing = androidGrabber->getIsCameraFacingFront();
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
