
#include "ofxSynthEnvelope.h"

ofxSynthADSR::ofxSynthADSR(){
	noteOn = false;
	a = d = s = r = 0;
}
void ofxSynthADSR::trigger(){
	offset = 0;
	noteOn = true;
}
void ofxSynthADSR::release(){
	noteOn = false;
}
void ofxSynthADSR::audioRequested( float* buffer, int numFrames, int numChannels ){
	for (int i = 0; i < numFrames; i++){
		if(offset<a){ // attack
			buffer[i*numChannels] = ((float)offset)/a;
			offset++;
		}else if (offset>a&&offset<a+d) { // decay
			buffer[i*numChannels] = ofLerp(1.0, s, ((float)offset-a)/d);
			offset++;
		}else if(noteOn){ // sustain
			buffer[i*numChannels] = s;
		}else if(offset<a+d+r){ // release
			buffer[i*numChannels] = ofLerp(s, 0.0, (float)(offset-a-d)/(float)r);
			offset++;
		}else {
			buffer[i*numChannels] = 0;
		}

		// copy to the other channels that are being requested
		for (int j = 1; j<numChannels; j++) {
			buffer[i*numChannels+j] = buffer[i*numChannels];
		}
	}
}
