#pragma once

#include "ofMain.h"

class ofxSynthFilter : public ofSoundEffect{
	public:
		void setup();
		string getName() { return "ofxSynthFilter"; }
		virtual void process( float* input, float *output, int numFrames, int numInChannels, int numOutChannels );
		void setCutoff(float _cutoff);
		void processSample(float *inputSample);
		void setRes(float _res);
	private:
	
	float f, p, q, cutoff, resonance;
	struct filterState {
		float b0, b1, b2, b3, b4;
		float t1, t2;
	};
	vector<filterState> filterStates;
	
		bool lowPass;
		void calc();
};