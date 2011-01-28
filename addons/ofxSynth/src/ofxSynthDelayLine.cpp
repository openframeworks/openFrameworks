#include "ofxSynthDelayLine.h"

ofxSynthDelayline::ofxSynthDelayline(){
	setSize(1.1);
	setFeedback(0.5);
	setMix(0.5);
}
void ofxSynthDelayline::process( float* input, float *output, int numFrames, int numInChannels, int numOutChannels )
{
	for (int i = 0; i < numFrames; i++){
		if (phase>=size) {
			phase=0;
		}
		memory[phase]=(memory[phase]*feedback)+(input[i*numInChannels]*feedback)*0.5;
		phase+=1;
		for (int j=0; j<numOutChannels; j++) {
			// puts the sound in memory on to all the output channels
			output[i*numOutChannels+j]=memory[phase+1]*mix+input[i*numInChannels]*(1.0-mix);
		}
	}
}
void ofxSynthDelayline::setSize(float _size){
	size = _size*sampleRate;
}
void ofxSynthDelayline::setFeedback(float _feedback){
	feedback = _feedback;
}
void ofxSynthDelayline::setMix(float _mix){
	mix = _mix;
}