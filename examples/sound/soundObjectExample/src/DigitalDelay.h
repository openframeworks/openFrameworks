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
 *  DigitalDelay.h, created by Marek Bereza on 11/08/2013.
 */

#pragma once

#include "ofMain.h"



#pragma once
#include <string.h>

#define kMAX_DELAY		44100


class DigitalDelay: public ofSoundObject {
public:
	
	
	
	vector<float> buffer;

	
	DigitalDelay() {
		setDelay(4410);
		feedback = 0.5f;
		mix = 0.5f;
	}
	
	void setDelay(int newDelay) {
		buffer.resize(newDelay);
		buffer.assign(newDelay, 0);
		pos = 0;
	}
	
	void setMix(float mix) {
		this->mix = mix;
	}
	
	void setFeedback(float feedback) {
		this->feedback = feedback;
	}
	
	

	float process(float in) {
		pos++;
		pos %= buffer.size();
		float out = buffer[pos];
		buffer[pos] = feedback*buffer[pos] + in;
		return in + (out - in)*mix;
	};
	
	void process(ofSoundBuffer &input, ofSoundBuffer &output) {
		
		for(int i = 0; i < output.size(); i++) {
			output[i] = process(input[i]);
		}
	}
	
	float feedback;
	float mix;

private:
	int pos;
	
};


