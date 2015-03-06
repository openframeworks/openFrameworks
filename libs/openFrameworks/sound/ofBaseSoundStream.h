#pragma once
#include "ofConstants.h"

class ofBaseSoundInput;
class ofBaseSoundOutput;
class ofSoundDevice;
class ofSoundBuffer;

class ofBaseSoundStream{
	public:
		virtual ~ofBaseSoundStream(){}
	
		virtual void setDeviceID(int deviceID) = 0;
		virtual bool setup(int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers)=0;
		virtual bool setup(ofBaseApp * app, int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers)=0;
		virtual void setInput(ofBaseSoundInput * soundInput) = 0;
		virtual void setOutput(ofBaseSoundOutput * soundOutput) = 0;
	
		virtual std::vector<ofSoundDevice> getDeviceList() const = 0;
		virtual void printDeviceList() const;
	
		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void close() = 0;

		virtual long unsigned long getTickCount() const = 0;
		virtual int getNumInputChannels() const = 0;
		virtual int getNumOutputChannels() const = 0;
		virtual int getSampleRate() const = 0;
		virtual int getBufferSize() const = 0;
		virtual int getDeviceID() const = 0;
};
