#pragma once

#include "ofConstants.h"

#include "ofBaseSoundStream.h"
#include "ofTypes.h"
#include "ofSoundBuffer.h"

class RtAudio;
typedef unsigned int RtAudioStreamStatus;

class ofRtAudioSoundStream : public ofBaseSoundStream{
	public:
		ofRtAudioSoundStream();
		~ofRtAudioSoundStream();
		
		void listDevices();
		void setDeviceID(int deviceID);
		void setInDeviceID(int deviceID);
		void setOutDeviceID(int deviceID);

		void setInput(ofBaseSoundInput * soundInput);
		void setOutput(ofBaseSoundOutput * soundOutput);
		bool setup(int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers);
		bool setup(ofBaseApp * app, int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers);
		
		void start();
		void stop();
		void close();
		
		long unsigned long getTickCount();		

		int getNumInputChannels();
		int getNumOutputChannels();
		int getSampleRate();
		int getBufferSize();
		int getDeviceID();
	
	private:
		long unsigned long tickCount;
		shared_ptr<RtAudio>	audio;
		int sampleRate;
		int outDeviceID;
		int inDeviceID;
		int bufferSize;
		int nInputChannels;
		int nOutputChannels;
		ofBaseSoundInput * soundInputPtr;
		ofBaseSoundOutput * soundOutputPtr;
		ofSoundBuffer inputBuffer;
		ofSoundBuffer outputBuffer;
	
		static int rtAudioCallback(void *outputBuffer, void *inputBuffer, unsigned int bufferSize, double streamTime, RtAudioStreamStatus status, void *data);

};


