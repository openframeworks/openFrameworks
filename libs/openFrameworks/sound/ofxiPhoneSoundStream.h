#pragma once

#include "ofConstants.h"

#include "ofBaseSoundStream.h"
#include "ofTypes.h"


class ofxiPhoneSoundStream : public ofBaseSoundStream{
	public:
		ofxiPhoneSoundStream();
		~ofxiPhoneSoundStream();
		
		void listDevices();
		void setDeviceID(int deviceID);

		void setInput(ofBaseSoundInput * soundInput);
		void setOutput(ofBaseSoundOutput * soundOutput);
		bool setup(int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers);
		bool setup(ofBaseApp * app, int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers);
		
		void start();
		void stop();
		void close();
		
		long unsigned long getTickCount();		 // always returns 0. not implemented on iphone
	
		int getNumInputChannels();
		int getNumOutputChannels();
		
	private:
		long unsigned long	tickCount;
		int					nInputChannels;
		int					nOutputChannels;
		int					sampleRate;
};


