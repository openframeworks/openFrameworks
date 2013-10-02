#pragma once

#include "ofConstants.h"
#include "ofBaseSoundStream.h"
#include "ofTypes.h"

typedef void PaStream;
struct PaStreamCallbackTimeInfo;
typedef unsigned long PaStreamCallbackFlags;

class ofPASoundStream : public ofBaseSoundStream{
	public:
		ofPASoundStream();
		~ofPASoundStream();
		
		void listDevices();
		void setDeviceID(int deviceID);

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
	
		
	private:
		long unsigned long	tickCount;
		PaStream *			audio;
		int					sampleRate;
		int					deviceID;
		int					nInputChannels;
		int					nOutputChannels;
		int					bufferSize;
		ofBaseSoundInput *  soundInputPtr;
		ofBaseSoundOutput * soundOutputPtr;
		
		static int paAudioCallback(const void *inputBuffer,
		        void *outputBuffer,
		        unsigned long bufferSize,
		        const PaStreamCallbackTimeInfo* streamTime,
		        PaStreamCallbackFlags status,
		        void *data);
		static bool initialized;
};


