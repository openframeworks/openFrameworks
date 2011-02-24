#pragma once

#include "ofMain.h"

class ofxSynthDelayline : public ofSoundEffect {
	public:
		
		ofxSynthDelayline();
		string getName() { return "ofxSynthDelayLine"; }
		virtual void process( float* input, float *output, int numFrames, int numInChannels, int numOutChannels );
		void setSize(float _size);
		void setFeedback(float _feedback);
		void setMix(float _mix);
		
	private:
		float frequency;
		int phase;
		float startphase;
		float endphase;
		float output;
		float memory[88200];
		int size;
		float feedback;
		float mix;
};
