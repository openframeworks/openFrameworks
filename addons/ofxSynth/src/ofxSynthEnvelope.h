
#pragma once
#include "ofSoundUnit.h"
#include "ofMath.h";

/** ofxSynthADSR
 
 produces a 3 point envelope with sustain
 
*/
class ofxSynthADSR : public ofSoundSource {
	public:
		ofxSynthADSR();
		void trigger(); // start the note, go to the sustain stage
		void release(); // continue past the sustain stage
		void setADSR(float _a, float _d, float _s, float _r){
			a=_a; d=_d; s=_s; r=_r;
		};
		void goToEnd(); // jump to the end of the envelope
		string getName() { return "ofxSynthADSR"; }
		void audioRequested( float* buffer, int numFrames, int numChannels );
	protected:
		float a, d, s, r;
		int offset;
		bool noteOn;
};
