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
class ofSoundStream;

class ofSoundMixer;
/// return the shared system ofSoundMixer that, by default is connected to ofSoundStreamGetSystemStream()
ofSoundMixer* ofSoundMixerGetSystemMixer();

class ofSoundMixer: public ofBaseSoundOutput {
public:
	ofSoundMixer();

	void setup( ofSoundStream* stream );

	void addSoundOutput(ofBaseSoundOutput & out, float volume=1, float pan=0);
	void removeSoundOutput(ofBaseSoundOutput & out);
	
	void setVolume(ofBaseSoundOutput & out, float volume);
	void setPan(ofBaseSoundOutput & out, float pan);
	
private:
	bool isSystemMixer() { return this == ofSoundMixerGetSystemMixer(); }
	void audioOut(float * output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount);
	
	typedef struct {
		ofBaseSoundOutput* sourceOutput;
		float volume;
		float pan;
		float volumeLeft;
		float volumeRight;
		bool operator==(const ofBaseSoundOutput*otherOutput) const { return otherOutput==sourceOutput; }
	} ofSoundMixerSource;
	vector<ofSoundMixerSource> sources;

	// helper method to locate a given source in the vecotr of sources
	vector<ofSoundMixerSource>::iterator findSource(ofBaseSoundOutput& out);
	

	ofSoundBuffer buffer;
	bool isSetup;
};

#endif /* OFSOUNDMIXER_H_ */
