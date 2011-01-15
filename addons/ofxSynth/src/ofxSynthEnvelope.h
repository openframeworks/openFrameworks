
#pragma once
#include "ofSoundUnit.h"
#include "ofMath.h";

/** ofxSynthADSR
 
 produces a 3 point envelope with sustain
 
*/
class ofxSynthADSR : public ofSoundSource {
	public:
		void trigger();
		void release();
		void setADSR(float _a, float _d, float _s, float _r){
			// TODO: make sure that we don't overrun if we set this too short all of a sudden
			// might be as simple as making the output level a smoothed float
			a=_a; d=_d; s=_s; r=_r;
		};
		string getName() { return "ofxSynthADSR"; }
		void audioRequested( float* buffer, int numFrames, int numChannels );
	protected:
		float a, d, s, r;
		int offset;
		bool noteOn;
};
