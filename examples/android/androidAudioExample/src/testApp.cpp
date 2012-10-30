#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
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
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
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
void testApp::keyPressed  (int key){ 
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	int width = ofGetWidth();
	pan = (float)x / (float)width;
	float height = (float)ofGetHeight();
	float heightPct = ((height-y) / height);
	targetFrequency = 2000.0f * heightPct;
	phaseAdderTarget = (targetFrequency / (float) sampleRate) * TWO_PI;
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

void testApp::audioRequested(float * output,int bufferSize,int nChannels){
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

void testApp::audioReceived(float * input,int bufferSize,int nChannels){
	for (int i = 0; i < bufferSize; i++){
		rAudio[i%256] = input[i*nChannels ] + input[i*nChannels +1] ;
	}

}
