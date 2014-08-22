#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(255,255,255);
	ofSetColor(0,0,0);
	ofSetLogLevel(OF_LOG_NOTICE);
	ofNoFill();
	sampleRate 			= 44100;
	phase 				= 0;
	phaseAdder 			= 0.0f;
	phaseAdderTarget 	= 0.0f;
	volume				= 0.1f;
	bNoise 				= false;
	lAudio = new float[256];
	rAudio = new float[256];

	soundStream.setup(this,2,2, sampleRate,256, 4);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	// draw the left:
	ofFill();
	ofSetHexColor(0x333333);
	ofRect(20,100,256,100);
	ofRect(20,250,256,100);
	ofSetHexColor(0xFFFFFF);
	ofNoFill();
	ofBeginShape();
	for (int i = 0; i < 256; i++){
		ofVertex(20+i,150+lAudio[i]*70.0f);
	}
	ofEndShape(false);

	ofBeginShape();
	for (int i = 0; i < 256; i++){
		ofVertex(20+i,300+rAudio[i]*70.0f);
	}
	ofEndShape(false);

	ofSetHexColor(0x000000);

	ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate()),20,20);

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
	int width = ofGetWidth();
	pan = (float)x / (float)width;
	float height = (float)ofGetHeight();
	float heightPct = ((height-y) / height);
	targetFrequency = 2000.0f * heightPct;
	phaseAdderTarget = (targetFrequency / (float) sampleRate) * TWO_PI;
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


void ofApp::audioRequested(float * output,int bufferSize,int nChannels){
	//pan = 0.5f;
	float leftScale = 1 - pan;
	float rightScale = pan;

	// sin (n) seems to have trouble when n is very large, so we
	// keep phase in the range of 0-TWO_PI like this:
	while (phase > TWO_PI){
		phase -= TWO_PI;
	}

	if ( bNoise == true){
		// ---------------------- noise --------------
		for (int i = 0; i < bufferSize; i++){
			lAudio[i] = output[i*nChannels    ] = ofRandomf() * volume * leftScale;
			rAudio[i] = output[i*nChannels + 1] = ofRandomf() * volume * rightScale;
		}
	} else {

		for (int i = 0; i < bufferSize; i++){
			phaseAdder = 0.6f * phaseAdder + 0.4f * phaseAdderTarget;
			phase += phaseAdder;
			float sample = sin(phase);
			lAudio[i%256] = output[i*nChannels    ] = sample * volume * leftScale;
			output[i*nChannels + 1] = sample * volume * rightScale;
		}
	}
}

void ofApp::audioReceived(float * input,int bufferSize,int nChannels){
	for (int i = 0; i < bufferSize; i++){
		rAudio[i%256] = input[i*nChannels ] + input[i*nChannels +1] ;
	}

}
