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
		bool lowPass;
	private:
		float f, p, q, cutoff, resonance;
		float b0, b1, b2, b3, b4;
		float t1, t2;
		void calc();
};