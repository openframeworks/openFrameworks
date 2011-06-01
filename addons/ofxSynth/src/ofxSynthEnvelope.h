
#pragma once
#include "ofBaseTypes.h"
#include "ofMath.h"

/** ofxSynthADSR
 
 produces a 3 point envelope with sustain
 
*/
class ofxSynthADSR: public ofBaseSoundOutput {
	public:
		ofxSynthADSR();
		void trigger(); // start the note, go to the sustain stage
		void release(); // continue past the sustain stage
		void setADSR(float _a, float _d, float _s, float _r){
			a=_a; d=_d; s=_s; r=_r;
			inv_a = 1./a; inv_d = 1./d; inv_s = 1./s; inv_r=1./r;
		};
		void goToEnd(){/*TODO:*/} ; // jump to the end of the envelope
		string getName() { return "ofxSynthADSR"; }
		void audioRequested( float* buffer, int numFrames, int numChannels );
	protected:
		float a, d, s, r;
		float inv_a, inv_d, inv_s, inv_r;
		int offset;
		bool noteOn;
};
