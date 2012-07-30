/*
 * ofSoundMixer.h
 *
 *  Created on: 25/07/2012
 *      Author: arturo
 */

#ifndef OFSOUNDMIXER_H_
#define OFSOUNDMIXER_H_

#include "ofBaseTypes.h"
#include "ofConstants.h"
#include "ofSoundBuffer.h"

class ofSoundMixer: public ofBaseSoundOutput {
public:
	ofSoundMixer();

	void setup(int bufferSize, int nChannels, int sampleRate);

	void addSoundOutput(ofBaseSoundOutput & out, float volume=1, float pan=0);
	void setVolume(ofBaseSoundOutput & out, float volume);
	void setPan(ofBaseSoundOutput & out, float pan);

private:
	void audioOut(float * output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount);
	vector<ofBaseSoundOutput*> sources;
	vector<float> volumes;
	vector<float> pans;
	vector<float> volumesLeft;
	vector<float> volumesRight;
	ofSoundBuffer buffer;
};

#endif /* OFSOUNDMIXER_H_ */
