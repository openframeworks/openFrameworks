#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(255,255,255);
	ofSetLogLevel(OF_LOG_NOTICE);
	ofSetOrientation(OF_ORIENTATION_90_LEFT);
	// this makes the camera directly return grayscale image, faster!
	grabber.setPixelFormat(OF_PIXELS_MONO);
	grabber.initGrabber(320,240);
	gray.allocate(grabber.getWidth(),grabber.getHeight());
	bg.allocate(grabber.getWidth(),grabber.getHeight());
	//diff.allocate(grabber.getWidth(),grabber.getHeight());
	//contourFinder.allocate(grabber.getWidth(),grabber.getHeight());
	one_second_time = ofGetSystemTime();
	camera_fps = 0;
	frames_one_sec = 0;
	captureBg = true;
}

//--------------------------------------------------------------
void testApp::update(){
	grabber.update();
	if(grabber.isFrameNew()){
		frames_one_sec++;
		if( ofGetSystemTime() - one_second_time >= 1000){
			camera_fps = frames_one_sec;
			frames_one_sec = 0;
			one_second_time = ofGetSystemTime();
		}
		gray.setFromPixels(grabber.getPixels(),grabber.getWidth(),grabber.getHeight());
		if(captureBg){
			bg = gray;
			captureBg = false;
		}

		//gray.absDiff(bg);
		gray.threshold(80);
		contourFinder.findContours(gray,10,grabber.getWidth()*grabber.getHeight()/3,10,true,true);
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(0xFFFFFF);
	gray.draw(5,5);
	contourFinder.draw(5,5);
	ofSetColor(0x000000);
	ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate()),330,10);
	ofDrawBitmapString("camera fps: " + ofToString(camera_fps),330,30);
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
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

