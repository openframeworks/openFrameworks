#include "ofApp.h"
#include "ofxAndroidVideoGrabber.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0,0,0);

	// List devices
	// The device name contains information about the direction of the camera
	vector<ofVideoDevice> devices = grabber.listDevices();
	for(int i=0;i<devices.size(); i++){
		ofLog()<<"Device "<<i<<": "<<devices[i].deviceName;
	}

	// Set the pixel format for getPixels to grayscale
	// (other image formats will include heavy colorspace conversion)
	grabber.setPixelFormat(OF_PIXELS_GRAY);

	// Hint the grabber if you don't need pixel data for better performance
	//((ofxAndroidVideoGrabber*)grabber.getGrabber().get())->setUsePixels(false);

	// Start the grabber
	grabber.setup(1280, 720);

    if(grabber.isInitialized()) {
        // Get the orientation and facing of the current camera
        cameraOrientation = ((ofxAndroidVideoGrabber *) grabber.getGrabber().get())->getCameraOrientation();
        cameraFacingFront = ((ofxAndroidVideoGrabber *) grabber.getGrabber().get())->getFacingOfCamera();
    }
}

//--------------------------------------------------------------
void ofApp::update(){
	grabber.update();
	if(grabber.isFrameNew()){
		cameraFps.newFrame();

		grabberImage.loadData(grabber.getPixels());
	}
	cameraFps.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	// Calculate aspect ratio of grabber image
	float grabberAspectRatio = grabber.getWidth() / grabber.getHeight();

	// Draw camera image centered in the window
	ofPushMatrix();
	ofSetColor(255);
	ofSetRectMode(OF_RECTMODE_CENTER);

	// Draw from the center of the window
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);

	if(cameraFacingFront) {
		// If the camera is front, then rotate clockwise
		ofRotateDeg(appOrientation);
	} else {
		// If the camera is backfacing, then rotate counter clockwise
		ofRotateDeg(-appOrientation);
	}

	// Rotate the cameras orientation offset
	ofRotateDeg(cameraOrientation);

	int width = ofGetWidth();
	int height = ofGetHeight();

	// If its landscape mode, then swap width and height
	if(appOrientation == 90 || appOrientation == 270){
		std::swap(width, height);
	}

	// Draw the image
	if(width < height) {
		grabber.draw(0,0, width * grabberAspectRatio,
					 width);
	} else {
		grabber.draw(0,0, height,
					 height * 1.0/grabberAspectRatio);
	}

	ofPopMatrix();
	ofSetRectMode(OF_RECTMODE_CORNER);

	// Draw text gui
	ofDrawRectangle(0, 0, 300, 120);
	ofSetColor(0);
	ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate()),20,20);
	ofDrawBitmapString("camera fps: " + ofToString(cameraFps.getFps()),20,40);

	if(cameraFacingFront) {
		ofDrawBitmapString("facing: front (click to swap)", 20, 60);
	}  else {
		ofDrawBitmapString("facing: back (click to swap)", 20, 60);
	}
	ofDrawBitmapString("camera orientation: " + ofToString(cameraOrientation) ,20,80);
	ofDrawBitmapString("app orientation: " + ofToString(appOrientation) ,20,100);

	// Draw the image through raw pixels
	ofSetColor(255);
	ofDrawRectangle(0, 130, 300, 20);
	ofSetColor(0);
	ofDrawBitmapString("raw pixel data" ,20,143);

	ofSetColor(255);
	grabberImage.draw(0,150, 300, 300*1.0/grabberAspectRatio);
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
	// Swap between back and front camera

	// Get the native android video grabber
	ofxAndroidVideoGrabber* androidGrabber = (ofxAndroidVideoGrabber*)grabber.getGrabber().get();

	// If the current camera is frontal, then choose the back camera
	if(cameraFacingFront) {
		androidGrabber->setDeviceID(androidGrabber->getBackCamera());
	} else {
		androidGrabber->setDeviceID(androidGrabber->getFrontCamera());
	}

	// Read current orientation and facing out again
	cameraOrientation = androidGrabber->getCameraOrientation();
	cameraFacingFront = androidGrabber->getFacingOfCamera();
}

void ofApp::deviceOrientationChanged(ofOrientation newOrientation){
	appOrientation = ofOrientationToDegrees(newOrientation);
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
