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

	void audioOutBuffersChanged(int bufferSize, int nChannels, int sampleRate);
	void audioOut(float * output, int nFrames, int nChannels, int deviceID, long unsigned long tickCount);

	typedef struct {
		ofBaseSoundOutput* sourceOutput;
		float volume;
		float pan;
		float volumeLeft;
		float volumeRight;
		bool operator==(const ofBaseSoundOutput*otherOutput) const { return otherOutput==sourceOutput; }
	} ofSoundMixerSource;

	ofSoundMixerSource& getSource( ofBaseSoundOutput& out );
	
	vector<ofSoundMixerSource> sources;
	vector<ofSoundMixerSource> sourceAddQueue;
	vector<ofSoundMixerSource> sourceRemoveQueue;



	ofSoundBuffer buffer;
	bool tellBuffersChanged;
	bool isSetup;
	
	ofMutex mutex;
};

#endif /* OFSOUNDMIXER_H_ */
