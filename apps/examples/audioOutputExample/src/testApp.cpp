#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	ofBackground(255,255,255);

	// 2 output channels,
	// 0 input channels
	// 22050 samples per second
	// 512 samples per buffer
	// 4 num buffers (latency)
	
	int bufferSize		= 512;
	sampleRate 			= 44100;
	phase 				= 0;
	phaseAdder 			= 0.0f;
	phaseAdderTarget 	= 0.0f;
	volume				= 0.1f;
	bNoise 				= false;
	lAudio = new float[bufferSize];
	rAudio = new float[bufferSize];
	
	//soundStream.listDevices();
	
	//if you want to set the device id to be different than the default
	//soundStream.setDeviceID(1); 	//note some devices are input only and some are output only 

	soundStream.setup(this, 2, 0, sampleRate, bufferSize, 4);
	
	ofSetFrameRate(60);
}


//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){


	// draw the left:
	ofSetHexColor(0x333333);
	ofRect(100,100,256,200);
	ofSetHexColor(0xFFFFFF);
	for (int i = 0; i < 256; i++){
		ofLine(100+i,200,100+i,200+lAudio[i]*100.0f);
	}

	// draw the right:
	ofSetHexColor(0x333333);
	ofRect(600,100,256,200);
	ofSetHexColor(0xFFFFFF);
	for (int i = 0; i < 256; i++){
		ofLine(600+i,200,600+i,200+rAudio[i]*100.0f);
	}

	ofSetHexColor(0x333333);
	char reportString[255];
	sprintf(reportString, "volume: (%f) modify with -/+ keys\npan: (%f)\nsynthesis: %s", volume, pan, bNoise ? "noise" : "sine wave");
	if (!bNoise) sprintf(reportString, "%s (%fhz)", reportString, targetFrequency);

	ofDrawBitmapString(reportString,80,380);
	ofDrawBitmapString("press 's' to unpause the audio 'e' to pause the audio", 80, 450);
	ofDrawBitmapString("ticks: " + ofToString(soundStream.getTickCount()), 80, 480);

}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	if (key == '-'){
		volume -= 0.05;
		volume = MAX(volume, 0);
	} else if (key == '+'){
		volume += 0.05;
		volume = MIN(volume, 1);
	}
	
	if( key == 's' ){
		soundStream.start();
	}
	
	if( key == 'e' ){
		soundStream.stop();
	}
	
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){

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
	int width = ofGetWidth();
	pan = (float)x / (float)width;
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	bNoise = true;
}


//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	bNoise = false;
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}
//--------------------------------------------------------------
void testApp::audioOut(float * output, int bufferSize, int nChannels){
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
		phaseAdder = 0.95f * phaseAdder + 0.05f * phaseAdderTarget;
		for (int i = 0; i < bufferSize; i++){
			phase += phaseAdder;
			float sample = sin(phase);
			lAudio[i] = output[i*nChannels    ] = sample * volume * leftScale;
			rAudio[i] = output[i*nChannels + 1] = sample * volume * rightScale;
		}
	}

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
