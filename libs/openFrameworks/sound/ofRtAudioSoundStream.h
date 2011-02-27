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
		bool setup(ofSoundStreamMode soundStreamMode, ofBaseHasSoundStream * hasSoundStream, int numChannels, int sampleRate, int bufferSize, int nBuffers);
		
		void start();
		void stop();
		void close();
		
		long unsigned long getTickCount();		
				
		int getNumInputChannels();
		int getNumOutputChannels();
		
		void audioReceived(float * buffer, int bufferSize, int nChannels);
		void audioRequested(float * buffer, int bufferSize, int nChannels);
	
		long unsigned long	tickCount;
		ofBaseHasSoundStream * hasSoundStreamPtr;
		
	protected:
		//----------------------------------- static variables:
		RtAudio							*		audio;
		int										sampleRate;
		int deviceID;
		int										nInputChannels;
		int										nOutputChannels;
		
};


