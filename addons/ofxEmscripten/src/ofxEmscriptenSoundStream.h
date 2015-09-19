/*
 * ofxEmscriptenSoundStream.h
 *
 *  Created on: May 16, 2014
 *      Author: arturo
 */

#pragma once

#include "ofBaseSoundStream.h"

class ofxEmscriptenSoundStream: public ofBaseSoundStream {
public:
	ofxEmscriptenSoundStream();
	~ofxEmscriptenSoundStream();

	std::vector<ofSoundDevice> getDeviceList() const;
	void setDeviceID(int deviceID);
	bool setup(int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers);
	bool setup(ofBaseApp * app, int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers);
	void setInput(ofBaseSoundInput * soundInput);
	void setOutput(ofBaseSoundOutput * soundOutput);

	void start();
	void stop();
	void close();

	long unsigned long getTickCount() const;
	int getNumInputChannels() const;
	int getNumOutputChannels() const;
	int getSampleRate() const;
	int getBufferSize() const;
	int getDeviceID() const;

private:
	static void audio_cb(int bufferSize, int inputChannels, int outputChannels, void * userData);
	void audioCB(int bufferSize, int inputChannels, int outputChannels);
	int context;
	int stream;
	unsigned long long tickCount;
	int bufferSize;
	int inChannels;
	int outChannels;
	vector<float> inbuffer;
	vector<float> outbuffer;
	ofBaseSoundInput* soundInput;
	ofBaseSoundOutput* soundOutput;
};
