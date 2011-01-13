#pragma once
#include "ofxMaxim.h"
#include "ofMain.h"
#include "ofxSynthDelayLine.h"
#include "ofxSynthFilter.h"

#define OFXSYNTHONESHOT 1
#define OFXSYNTHADR 2

const int wav_header_size = 0x2C;

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

class ofxSynth : public ofSoundSource{
	public:
		ofxSynth();
		string getName() { return "ofxSynth"; }
		void setup();
		
		void audioRequested( float* buffer, int numFrames, int numChannels );
		
		void trigger();
		void setFrequency(float freq);
		void setFilter(float _cutoff, float _res);
		void setFilterMode(int mode);
		void setFrequencyMidiNote(float note);
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

/** ofxSynthWaveWriter
 
 Class to write sample streams to the file system.
 Automatically writes to the system when the buffers are filled.
 
 */
 
class ofxSynthWaveWriter : public ofSoundEffectPassthrough {
	public:
		string getName() { return "ofxSynthWaveWriter"; }
		bool startWriting(string filename = "out.wav");
		void stopWriting();
		virtual void process( float* input, float *output, int numFrames, int numInChannels, int numOutChannels );
	private:
		void flush();

		bool writing;
		int offset, numChannels, numFrames;
		
		// the copied stuff from this example: https://github.com/nevyn/game-music-emu
		enum { buf_size = 32768 * 2 };
		unsigned char* buf;
		FILE*   file;
		long    sample_count_;
		long    rate;
		long    buf_pos;
		int     chan_count;

};