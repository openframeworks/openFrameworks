#pragma once

#include "ofConstants.h"
#include "ofBaseSoundStream.h"

class RtAudio;

class ofRtAudioSoundStream : public ofBaseSoundStream{
	public:
		ofRtAudioSoundStream();
		~ofRtAudioSoundStream();
		
		void listDevices();
		void setDeviceID(int deviceID);

		bool setupInput(ofBaseSoundInput * hasSoundStream, int numChannels, int sampleRate, int bufferSize, int nBuffers);
		bool setupOutput(ofBaseSoundOutput * hasSoundStream, int numChannels, int sampleRate, int bufferSize, int nBuffers);
		
		void start();
		void stop();
		void close();
		
		void incrementTickCount();
		
		long unsigned long getTickCount();		
				
		int getNumInputChannels();
		int getNumOutputChannels();
		
		void audioIn(float * buffer, int bufferSize, int nChannels);
		void audioOut(float * buffer, int bufferSize, int nChannels);
	
		ofBaseSoundInput * soundInputPtr;
		ofBaseSoundOutput * soundOutputPtr;
		
	protected:
		long unsigned long	tickCount;
		RtAudio *			audio;
		int					sampleRate;
		int					deviceID;
		int					nInputChannels;
		int					nOutputChannels;
		
};


