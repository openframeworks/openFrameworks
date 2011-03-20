#include "ofxSynth.h"

maxiOsc::maxiOsc(){
	phase = 0.0;
}

void maxiOsc::setSampleRate(int rate){
	sampleRate = rate;
}

ofxSynth::ofxSynth(){

	currentFrequency = startFrequency = targetFrequency = 440;
	
	setFrequency(440);
	currentAmp = 0;
	noteTime = 0;
	usesEnv = false;
	gain = 0.1;
	sustain = 0.5;

	modEnv.setADSR(44100*0.01, 44100*0.2, 0.0, 44100.0*0.1);
	env.setADSR(0.01*44100, 0.1*44100, 0.5, 0.93*44100);
	filter.setup();
	setFilter(0.7, 0.5);
	setFilterLowPass();
	waveMode = 0;
	portamento = 0;
	
}

void ofxSynth::audioOut( float* buffer, int numFrames, int numChannels,long unsigned long tickCount  ){
	// fill the required inputs
	float envBuffer[numFrames];
	float modEnvBuffer[numFrames];
	wave.setSampleRate(sampleRate);
	
	env.audioRequested(envBuffer, numFrames, 1);
	modEnv.audioRequested(modEnvBuffer, numFrames, 1); // we are only going to update once per buffer
	float currValue;
	float *buffer_ptr = buffer;
	for (int i = 0; i<numFrames; i++) {
		noteTime++;
		currentFrequency = ofLerp(startFrequency, targetFrequency, MIN((float)noteTime, portamento+1)/(float)(portamento+1));
		currentAmp = envBuffer[i];
		
		if (currentAmp > 1) {
			currentAmp = 1;
		}
		
		// load the proper waveform
		switch (waveMode) {
			case 0:
				currValue = wave.square(currentFrequency);
				break;
			case 1:
				currValue = wave.triangle(currentFrequency, 0);
				break;
			case 2:
				currValue = wave.saw(currentFrequency);
				break;
			default:
				currValue = 0;
				break;
		}
		currValue *= currentAmp;
		for (int j=0; j<numChannels; j++) {
			(*buffer_ptr++) = currValue;
		}
	}
	if (filterMode != 0) {
		filter.setCutoff(modEnvBuffer[0]*cutoff);
		filter.process(buffer, buffer, numFrames, numChannels, numChannels);
	}
}

void ofxSynth::trigger(){
	env.trigger();
	env.release();
	modEnv.trigger();
	modEnv.release();
}

void ofxSynth::setFilter(float _cutoff, float _res){
	cutoff = _cutoff;
	res = _res;
	filter.setCutoff(_cutoff);
	filter.setRes(_res);
}

void ofxSynth::setFrequency(float freq){
	startFrequency = currentFrequency;
	targetFrequency = freq;
	noteTime = 0;
}

void ofxSynth::setFrequencyMidiNote(float note){
	setFrequency(440.0*pow(2.0, (note-60.0)/12.0));
}

void ofxSynth::setSampleRate( int rate ){
	sampleRate = rate;
}
