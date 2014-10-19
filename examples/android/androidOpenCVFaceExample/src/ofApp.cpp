#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(255,255,255);
	ofSetLogLevel(OF_LOG_NOTICE);
	ofSetOrientation(OF_ORIENTATION_90_LEFT);

	int grabberWidth = 320;
	int grabberHeight = 240;
	grabber.setup(grabberWidth,grabberHeight);

	colorCv.allocate(grabberWidth, grabberHeight);
	colorCvSmall.allocate(grabberWidth/4, grabberHeight/4);
	grayCv.allocate(grabberWidth/4, grabberHeight/4);
	
	faceFinder.setup("haarcascade_frontalface_default.xml");
	faceFinder.setNeighbors(1);
	faceFinder.setScaleHaar(1.5);

	one_second_time = ofGetSystemTime();
	camera_fps = 0;
	frames_one_sec = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
	grabber.update();
	if(grabber.isFrameNew()){
		frames_one_sec++;
		if( ofGetSystemTime() - one_second_time >= 1000){
			camera_fps = frames_one_sec;
			frames_one_sec = 0;
			one_second_time = ofGetSystemTime();
		}
		colorCv = grabber.getPixels();
		colorCvSmall.scaleIntoMe(colorCv, CV_INTER_NN);
		grayCv = colorCvSmall;
		faceFinder.findHaarObjects(grayCv);
		faces = faceFinder.blobs;
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetHexColor(0xFFFFFF);

	grabber.draw(0, 0);
	float scaleFactor = 4.0;

	ofPushStyle();
	ofNoFill();
	ofSetColor(255, 0, 255);
	for (int i = 0; i < faces.size(); i++) {
		ofxCvBlob& face = faces[i];
		ofRectangle rect(face.boundingRect.x * scaleFactor, face.boundingRect.y * scaleFactor, face.boundingRect.width * scaleFactor, face.boundingRect.height * scaleFactor);
		ofDrawRectangle(rect);
	}
	ofPopStyle();

	ofSetHexColor(0x000000);
	ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate()),330,10);
	ofDrawBitmapString("camera fps: " + ofToString(camera_fps),330,30);
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
