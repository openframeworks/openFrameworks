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

//     Ask for permission to record audio,
//     not needed if no in channels used
    ofxAndroidRequestPermission(OFX_ANDROID_PERMISSION_RECORD_AUDIO);

    ofSoundStreamSettings settings;
    settings.setOutListener(this);
    settings.setInListener(this);
    settings.numOutputChannels = 2;
    settings.numInputChannels = 2;
    settings.numBuffers = 4;
    settings.bufferSize = 256;
	soundStream.setup(settings);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	// draw the left:
	ofFill();
	ofSetHexColor(0x333333);
	ofDrawRectangle(20,100,256,100);
	ofDrawRectangle(20,250,256,100);
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


void ofApp::audioOut(ofSoundBuffer & buffer){
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
		for (int i = 0; i < buffer.getNumFrames(); i++){
			lAudio[i] = buffer.getSample(i, 0) = ofRandomf() * volume * leftScale;
			rAudio[i] = buffer.getSample(i, 1) = ofRandomf() * volume * rightScale;
		}
	} else {

		for (int i = 0; i < buffer.getNumFrames(); i++){
			phaseAdder = 0.6f * phaseAdder + 0.4f * phaseAdderTarget;
			phase += phaseAdder;
			float sample = sin(phase);
			lAudio[i%256] = buffer.getSample(i, 0) = sample * volume * leftScale;
            buffer.getSample(i, 1) = sample * volume * rightScale;
		}
	}
}

void ofApp::audioIn(ofSoundBuffer & buffer){
	for (int i = 0; i < buffer.getNumFrames(); i++){
		rAudio[i%256] = buffer.getSample(i, 0) + buffer.getSample(i, 1) ;
	}

}
