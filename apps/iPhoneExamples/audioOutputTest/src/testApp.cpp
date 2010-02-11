#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	playSound = false;
	inputVolume = 0;
	
#ifdef USING_INPUT
	// iphone has mono input only, so this is one input, 2 outputs
	ofSoundStreamSetup(0,1,this, SAMPLERATE, BUFFER_SIZE, 4);
#else
	ofSoundStreamSetup(2,0,this, SAMPLERATE, BUFFER_SIZE, 4);
#endif
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw() {
	// make it a little more colourful
	if(playSound) ofBackground((int)(leftWave.phaseIncrement*500.f)%255, (int)(rightWave.phaseIncrement*500.f)%255, inputVolume*255.f);
	else ofBackground(0, 0, 0);
	ofSetColor(0xFFFFFF);
#ifdef USING_INPUT
	for(int i = 1; i < 320 && i < BUFFER_SIZE; i++) {
		ofLine(i-1, 240.f*(1.f+buffer[i-1]), i, 240.f*(1.f+buffer[i]));
	}
#endif
	ofSetColor(0xFF0000);
	ofLine(0, 240, 320, 240);
}

#ifdef USING_INPUT
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
#endif

void testApp::audioRequested( float * output, int bufferSize, int nChannels ) {
#ifdef USING_INPUT
	//if(inputVolume>0.2) playSound = true;
#endif
	
	if(/*playSound && */nChannels==2) { // only do it if we've got 2 channels requested
		for(int i = 0; i < bufferSize; i++) {
#ifdef USING_INPUT
			// ring modulator
			output[i*2] = 0.4 * leftWave.getSample() *buffer[i]; // turn the volume of it down a bit
			output[i*2 + 1] = 0.4 *rightWave.getSample()*buffer[i];
#else
			// just output the samples to each side
			output[i*2] = leftWave.getSample()*0.3f; // turn the volume of it down a bit
			output[i*2 + 1] = rightWave.getSample()*0.3f;
#endif
		}
	} else { // output silence
		for(int i = 0; i < bufferSize*nChannels; i++) {
			output[i] = 0.f;
		}
		
		
	}
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	mousePressed(x, y, button);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	playSound = true;
	leftWave.setFrequency(100+x*6);
	rightWave.setFrequency(100+y*6);
}

//--------------------------------------------------------------
void testApp::mouseReleased(){
	playSound = false;
}
