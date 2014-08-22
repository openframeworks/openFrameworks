#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetOrientation(OF_ORIENTATION_90_LEFT);
	synth.loadSound("sounds/synth.wav");
	beats.loadSound("sounds/1085.mp3");
	vocals.loadSound("sounds/Violet.mp3");
	synth.setVolume(0.75f);
	beats.setVolume(0.75f);
	vocals.setVolume(0.5f);
	font.loadFont("Sudbury_Basin_3D.ttf", 32);
	beats.setMultiPlay(false);
	vocals.setMultiPlay(true);
}

//--------------------------------------------------------------
void ofApp::update(){
	// update the sound playing system:
	ofSoundUpdate();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(255,255,255);
	
	char tempStr[255];
	
	// draw the background colors:
	float widthDiv = ofGetWidth() / 3.0f;
	ofSetHexColor(0xeeeeee);
	ofRect(0,0,widthDiv,ofGetHeight()); 
	ofSetHexColor(0xffffff);
	ofRect(widthDiv,0,widthDiv,ofGetHeight()); 
	ofSetHexColor(0xdddddd);
	ofRect(widthDiv*2,0,widthDiv,ofGetHeight()); 
	

	//---------------------------------- synth:
	if (synth.getIsPlaying()) ofSetHexColor(0xFF0000);
	else ofSetHexColor(0x000000);
	font.drawString("synth !!", 50,50);
	
	ofSetHexColor(0x000000);
	sprintf(tempStr, "click to play\npct done: %f\nspeed: %f\npan: %f", synth.getPosition(),  synth.getSpeed(), synth.getPan());
	ofDrawBitmapString(tempStr, 50,ofGetHeight()-50);



	//---------------------------------- beats:
	if (beats.getIsPlaying()) ofSetHexColor(0xFF0000);
	else ofSetHexColor(0x000000);
	font.drawString("beats !!", widthDiv+50,50);

	ofSetHexColor(0x000000);
	sprintf(tempStr, "click and drag\npct done: %f\nspeed: %f", beats.getPosition(),  beats.getSpeed());
	ofDrawBitmapString(tempStr, widthDiv+50,ofGetHeight()-50);

	//---------------------------------- vocals:
	if (vocals.getIsPlaying()) ofSetHexColor(0xFF0000);
	else ofSetHexColor(0x000000);
	font.drawString("vocals !!", widthDiv*2+50,50);

	ofSetHexColor(0x000000);
	sprintf(tempStr, "click to play (multiplay)\npct done: %f\nspeed: %f\npan: %f", vocals.getPosition(),  vocals.getSpeed(), vocals.getPan());
	ofDrawBitmapString(tempStr, widthDiv*2+50,ofGetHeight()-50);
	
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
	float widthStep = ofGetWidth() / 3.0f;
	if (x < widthStep){
		float pct = x / widthStep;
		synth.play();
		synth.setSpeed( 0.1f + ((float)(ofGetHeight() - y) / (float)ofGetHeight())*2.0f);
		synth.setPan(2.0*pct-1.0);
	} else if (x >= widthStep && x < widthStep*2){
		beats.play();
	} else {
		vocals.play();
		vocals.setSpeed( 0.1f + ((float)(ofGetHeight() - y) / (float)ofGetHeight())*2.0f);
		vocals.setPan(2.0*(float)(x - widthStep*2) / (float)widthStep-1.0);
	}
}

//--------------------------------------------------------------
void ofApp::touchMoved(int x, int y, int id){
	// continuously control the speed of the beat sample via drag,
	// when in the "beat" region:
	float widthStep = ofGetWidth() / 3.0f;
	if (x >= widthStep && x < widthStep*2){
		beats.setSpeed( 0.5f + ((float)(ofGetHeight() - y) / (float)ofGetHeight())*1.0f);
	}
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
