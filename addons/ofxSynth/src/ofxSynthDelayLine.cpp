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
		memory[phase]=(memory[phase]*feedback)+((*input)*feedback)*0.5;
		phase+=1;
		for (int j=0; j<numOutChannels; j++) {
			// puts the sound in memory on to all the output channels
			*output++=memory[phase+1]*mix+(*input)*(1.0-mix);
		}
		input+=numInChannels;
	}
}
void ofxSynthDelayline::setSize(float _size){
	if(_size>2) _size=2;
	size = _size*sampleRate;
}
void ofxSynthDelayline::setFeedback(float _feedback){
	feedback = _feedback;
}
void ofxSynthDelayline::setMix(float _mix){
	mix = _mix;
}
