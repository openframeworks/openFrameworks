#pragma once

#include "ofConstants.h"

#include "ofBaseSoundStream.h"
#include "ofTypes.h"


class ofxiPhoneSoundStream : public ofBaseSoundStream{
	public:
		ofxiPhoneSoundStream();
		~ofxiPhoneSoundStream();
		
		/// these are not implemented on iOS
		void listDevices();
		void setDeviceID(int deviceID);

		void setInput(ofBaseSoundInput * soundInput);
		void setOutput(ofBaseSoundOutput * soundOutput);
		
		/// currently, the number of buffers is always 1 on iOS and setting nBuffers has no effect
		bool setup(int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers);
		bool setup(ofBaseApp * app, int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers);
		
		void start();
		void stop();
		void close();
		
		// not implemented on iOS, always returns 0
		long unsigned long getTickCount();
	
		int getNumInputChannels();
		int getNumOutputChannels();
		
	private:
		long unsigned long	tickCount;
		int					nInputChannels;
		int					nOutputChannels;
		int					sampleRate;
};


