#pragma once
#include "ofxMaxim.h"
#include "ofMain.h"

#define OFXSYNTHONESHOT 1
#define OFXSYNTHADR 2

class ofxSynthEnv{
	public:
		virtual float getLevel(){
			return 1.0;
		};
		virtual void gateOpen(){};
		virtual void gateClose(){};
	protected:
		float level;
		int offset;
};

class ofxSynthEnvAHD : public ofxSynthEnv{
	public:
		void gateOpen();
		float getLevel();
		float inTime, holdTime, outTime;
};

class ofxSynthFilter{
	public:
		float f, p, q, cutoff, resonance;
		float b0, b1, b2, b3, b4;
		float t1, t2;
		bool lowPass;
		void setup();
		void setCutoff(float _cutoff);
		void setRes(float _res);
		void calc();
		void processSample(float *sample);
};

class ofxSynthDelayline : public ofSoundEffect{
private:
	double frequency;
	int phase;
	double startphase;
	double endphase;
	double output;
	double memory[88200]; //TODO: make this change with a constructor 
	int size;
	float feedback;
	float mix;
public:
	ofxSynthDelayline(){setSize(1.1);setFeedback(0.5);}
	string getName() { return "ofxSynthDelayLine"; }
	virtual void process( float* input, float *output, int numFrames, int numInChannels, int numOutChannels );
	void setSize(float _size);
	void setFeedback(float _feedback);
};


class ofxSynth{
	public:
		ofxSynth();
		void setup();
		
		void audioRequested(float * input, int bufferSize);
		
		void trigger();
		void setFrequency(float freq);
		void setFilter(float _cutoff, float _res);
		void setFilterMode(int mode);
		void setNote(float note);
		void setWave(int _wavType);
		void setADRVol(float atk, float dec, float rel);
		void setADRMod(float atk, float dec, float rel);
		
		ofxMaxiOsc wave;
		ofxSynthEnvAHD env, modEnv;
		ofxSynthFilter filter;
		int ampMode;
		bool usesEnv, hasTrigger;
		
	protected:
		float currentFrequency, currentAmp, sustain, gain, cutoff, res, filterMod, pitchMod;
		int filterMode, waveMode;
		double ampEnv[8];
};
class ofxSynthSampler : public ofxSynth{
	public:
		void audioRequested(float * input, int bufferSize);
		void setNote(float note);
		void loadFile(string file);
	private:
		ofxMaxiSample sample;
};