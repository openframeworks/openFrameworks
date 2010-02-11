#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	// iphone has mono input only, so this is one input
	ofSoundStreamSetup(0,1,this, SAMPLERATE, BUFFER_SIZE, 4);
	
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw() {
	// make it a little more colourful
	ofBackground(0, 0, 0);
	ofSetColor(0xFFFFFF);
	for(int i = 1; i < 320 && i < BUFFER_SIZE; i++) {
		ofLine(i-1, 240.f*(1.f+buffer[i-1]), i, 240.f*(1.f+buffer[i]));
	}
}

void testApp::audioReceived( float * input, int bufferSize, int nChannels ) {
	if(nChannels==1) {
		float total = 0;
		for(int i = 0; i < bufferSize; i++) {
			buffer[i] = input[i];
			total += ABS(input[i]);
		}
		inputVolume = total/bufferSize;
	}
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(){
}
