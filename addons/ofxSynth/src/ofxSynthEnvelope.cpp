
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
	float currValue;
	for (int i = 0; i < numFrames; i++){
		if(offset<a){ // attack
			currValue = ((float)offset)*inv_a;
			offset++;
		}else if (offset>a&&offset<a+d) { // decay
			currValue = ofLerp(1.0, s, ((float)offset-a)*inv_d);
			offset++;
		}else if(noteOn){ // sustain
			currValue = s;
		}else if(offset<a+d+r){ // release
			currValue = ofLerp(s, 0.0, (float)(offset-a-d)*inv_r);
			offset++;
		}else {
			currValue = 0;
		}

		// copy to the other channels that are being requested
		for (int j = 0; j<numChannels; j++) {
			*buffer++ = currValue;
		}
	}
}
