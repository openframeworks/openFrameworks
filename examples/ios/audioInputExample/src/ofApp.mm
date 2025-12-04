/* Please refer to the README.md in the example's root folder for more information on usage */

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);
	ofBackground(255);
	ofSetOrientation(OF_ORIENTATION_90_RIGHT);//Set iOS to Orientation Landscape Right

	drawCounter = 0;
	bufferCounter = 0;

//	sound.load("sounds/beat.caf");
//	sound.setLoop(true);
//	sound.play();
//	sound.setVolume(0);
	
	ofSoundStreamSettings settings;
	settings.setInListener(this);
	settings.sampleRate = 44100;
	settings.numOutputChannels = 0;
	settings.numInputChannels = 1;
	settings.numBuffers = 1;
	settings.bufferSize = bufferSize;
	soundStream.setup(settings);

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	drawCounter++;
	
	ofPushStyle();
	ofSetColor(0);
	ofSetLineWidth(2);
	
	float y1 = ofGetHeight() * 0.5;
	ofDrawLine(0, y1, ofGetWidth(), y1);
	
	for(int i=0; i<bufferSize; i++){
		float p = i / (float)(bufferSize-1);
		float x = p * ofGetWidth();
		float y2 = y1 + buffer[i] * 200;

		ofDrawLine(x, y1, x, y2);
	}
	ofPopStyle();

	ofPushStyle();
	ofSetColor(0);
	ofDrawBitmapString("touch to play sound.", 20, ofGetHeight() - 60);
	ofDrawBitmapString("buffers received: " + ofToString(bufferCounter), 20, ofGetHeight() - 40);
	ofDrawBitmapString("draw routines called: " + ofToString(drawCounter), 20, ofGetHeight() - 20);
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::audioIn(ofSoundBuffer & input){
	for(int i=0; i<input.getNumFrames(); i++) {
		buffer[i] = input[i];
	}
	
	bufferCounter++;
}

//--------------------------------------------------------------
void ofApp::exit(){
	//
}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){
//	sound.setVolume(1.0);
}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch){
//	sound.setVolume(0.0);
}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){
	
}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchCancelled(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::lostFocus(){
	
}

//--------------------------------------------------------------
void ofApp::gotFocus(){
	
}

//--------------------------------------------------------------
void ofApp::gotMemoryWarning(){
	
}

//--------------------------------------------------------------
void ofApp::deviceOrientationChanged(int newOrientation){
	
}

