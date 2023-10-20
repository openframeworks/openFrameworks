/*
 * ofxEmscriptenSoundStream.h
 *
 *  Created on: May 16, 2014
 *      Author: arturo
 */

#pragma once

#include "ofSoundBaseTypes.h"
#include "ofSoundBuffer.h"

class ofxEmscriptenSoundStream: public ofBaseSoundStream {
public:
	ofxEmscriptenSoundStream();
	~ofxEmscriptenSoundStream();

	std::vector<ofSoundDevice> getDeviceList(ofSoundDevice::Api api) const;
	bool setup(const ofSoundStreamSettings & settings);
	void setInput(ofBaseSoundInput * soundInput);
	void setOutput(ofBaseSoundOutput * soundOutput);

	ofSoundDevice getInDevice() const;
	ofSoundDevice getOutDevice() const;

	void start();
	void stop();
	void close();

	uint64_t getTickCount() const;
	int getNumInputChannels() const;
	int getNumOutputChannels() const;
	int getSampleRate() const;
	int getBufferSize() const;

private:
	static void audio_cb(int bufferSize, int inputChannels, int outputChannels, void * userData);
	void audioCB(int bufferSize, int inputChannels, int outputChannels);
	int context;
	unsigned long long tickCount;
	ofSoundStreamSettings settings;
	ofSoundBuffer inbuffer;
	ofSoundBuffer outbuffer;
};
