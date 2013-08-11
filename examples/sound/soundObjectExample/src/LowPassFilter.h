/**     ___           ___           ___                         ___           ___     
 *     /__/\         /  /\         /  /\         _____         /  /\         /__/|    
 *    |  |::\       /  /::\       /  /::|       /  /::\       /  /::\       |  |:|    
 *    |  |:|:\     /  /:/\:\     /  /:/:|      /  /:/\:\     /  /:/\:\      |  |:|    
 *  __|__|:|\:\   /  /:/~/::\   /  /:/|:|__   /  /:/~/::\   /  /:/  \:\   __|__|:|    
 * /__/::::| \:\ /__/:/ /:/\:\ /__/:/ |:| /\ /__/:/ /:/\:| /__/:/ \__\:\ /__/::::\____
 * \  \:\~~\__\/ \  \:\/:/__\/ \__\/  |:|/:/ \  \:\/:/~/:/ \  \:\ /  /:/    ~\~~\::::/
 *  \  \:\        \  \::/          |  |:/:/   \  \::/ /:/   \  \:\  /:/      |~~|:|~~ 
 *   \  \:\        \  \:\          |  |::/     \  \:\/:/     \  \:\/:/       |  |:|   
 *    \  \:\        \  \:\         |  |:/       \  \::/       \  \::/        |  |:|   
 *     \__\/         \__\/         |__|/         \__\/         \__\/         |__|/   
 *
 *  Description: 
 *				 
 *  LowPassFilter.h, created by Marek Bereza on 11/08/2013.
 */

#pragma once

#include "ofMain.h"

class LowPassFilter: public ofSoundObject {
	
public:
	
	// originally https://github.com/micknoise/Maximilian/blob/master/ofxMaxim/ofxMaxim/libs/maximilian.h
	// ported to js, ported back again
	class LPF {
	public:
		float z;
		float x;
		float y;
		float r;
		float c;
		LPF() {
			z = x = y = r = c = 0;
		}
#define SQRT2 1.414213562f
#define TWOPI 6.2831853071f
#define TWOPI_BY_SAMPLERATE 0.00014247585731f
		// cutoff in hz/2 (min 10Hz/2), resonance 1 to 10
		float processSample(float input) {
	  		
		    x += (input - y)*c;
		    y += x;
		    x *= r;
		    //return y;
			return x;
	 	}
		void setParameters(float cutoff, float resonance) {
			if(cutoff<50) cutoff = 50;
			if(cutoff>11025) cutoff = 11025;
	  		z=cos(TWOPI_BY_SAMPLERATE*cutoff);
		    c = 2 - 2*z;
		    float zzz = z-1;
		    zzz = zzz*zzz*zzz;
		    r = (SQRT2*sqrt(-zzz)+resonance*(z-1))/(resonance*(z-1));
		}

		// sampleStep is how many frames to jump each frame.
		// lengthInSamples is the whole sample length, in frames*numChannels
		//			i.e. number of floats in "in" and "out"
		void process(float *in, float *out, int lengthInSamples, int sampleStep) {
			for(int i = 0; i < lengthInSamples; i += sampleStep) {
				out[i] = processSample(in[i]);
			}
		}
		
	private:
		float buf0, buf1;
		float q, f, fb;
	};
	
	
	LowPassFilter() {
		res = 0;
		fc = 1;
	}
	void process(ofSoundBuffer &input, ofSoundBuffer &output) {
		
		int numChannels = output.getNumChannels();
		while(filters.size()<numChannels) {
			filters.push_back(LPF());
		}
		
		for(int i = 0; i < numChannels; i++) {
			filters[i].setParameters(fc, 1+res);
			filters[i].process(&input[i], &output[i], output.size(), numChannels);
		}
		
	}
	
	void setParameters(float fc, float res) {
		this->fc = fc;
		this->res = res;
	}
	
	float fc;
	float res;
	
	vector<LPF> filters;
};

