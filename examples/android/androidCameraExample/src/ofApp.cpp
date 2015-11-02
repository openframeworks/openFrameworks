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

	// Set the pixel format for getPixels to monochrome
	// (other image formats will include heavy pixel conversion)
	grabber.setPixelFormat(OF_PIXELS_MONO);

	// Hint the grabber if you don't need pixel data for better performance
	//((ofxAndroidVideoGrabber*)grabber.getGrabber().get())->setUsePixels(false);

	// Start the grabber
	grabber.setup(1280,960);

	// Get the orientation and facing of the current camera
	orientation = ((ofxAndroidVideoGrabber*)grabber.getGrabber().get())->getCameraOrientation();
	facing = ((ofxAndroidVideoGrabber*)grabber.getGrabber().get())->getFacingOfCamera();

	one_second_time = ofGetElapsedTimeMillis();
	camera_fps = 0;
	frames_one_sec = 0;
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

		grabberImage.setFromPixels(grabber.getPixels());
	}
}

//--------------------------------------------------------------
void ofApp::draw(){    
	// Calculate aspect ratio of grabber image
	float grabberAspectRatio = grabber.getWidth() / grabber.getHeight();

	// Draw camera image centered in the window
	ofPushMatrix();
	ofSetHexColor(0xFFFFFF);
	ofSetRectMode(OF_RECTMODE_CENTER);

	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);

	int wOrientation = ofOrientationToDegrees(ofGetOrientation());
	ofLogNotice()<<"Orientation: "<<ofGetOrientation()<<endl;

	if(ofGetWidth() > ofGetHeight()) {
		grabber.draw(0,0, ofGetHeight() * grabberAspectRatio,
					 ofGetHeight());
	} else {
		grabber.draw(0,0, ofGetWidth(),
					 ofGetWidth()  * 1.0/grabberAspectRatio);

	}
	ofPopMatrix();
	ofSetRectMode(OF_RECTMODE_CORNER);

	// Draw the image through raw pixels
	grabberImage.draw(0,110, 300, 300*1.0/grabberAspectRatio);

	// Draw text gui
	ofDrawRectangle(0, 0, 300, 100);
	ofSetHexColor(0x000000);
	ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate()),20,20);
	ofDrawBitmapString("camera fps: " + ofToString(camera_fps),20,40);

	if(facing == 1) {
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
	// Swap between back and front camera

	// Get the native android video grabber
	ofxAndroidVideoGrabber* androidGrabber = (ofxAndroidVideoGrabber*)grabber.getGrabber().get();

	// If the current camera is frontal, then choose the back camera
	if(facing == 1) {
		androidGrabber->setDeviceID(androidGrabber->getBackCamera());
	} else {
		androidGrabber->setDeviceID(androidGrabber->getFrontCamera());
	}

	// Read current orientation and facing out again
	orientation = androidGrabber->getCameraOrientation();
	facing = androidGrabber->getFacingOfCamera();
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
