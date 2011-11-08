#pragma once
#include "ofConstants.h"
#include "ofTypes.h"

class ofBaseSoundInput;
class ofBaseSoundOutput;

class ofBaseSoundStream{
	public:
		virtual ~ofBaseSoundStream(){}

		virtual vector<ofStreamDeviceInfo> listDevices() = 0;
		virtual bool setDeviceID(int deviceID) = 0;
		virtual bool setDeviceID(string deviceName) = 0;
		virtual bool setup(int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers)=0;
		virtual bool setup(ofBaseApp * app, int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers)=0;
		virtual void setInput(ofBaseSoundInput * soundInput) = 0;
		virtual void setOutput(ofBaseSoundOutput * soundOutput) = 0;

		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void close() = 0;

		virtual long unsigned long getTickCount() = 0;
};
