#pragma once
#include "ofConstants.h"

class ofBaseSoundInput;
class ofBaseSoundOutput;

class ofBaseSoundStream{
	public:
		virtual ~ofBaseSoundStream(){}
		
		virtual void listDevices() = 0;
		virtual void setDeviceID(int deviceID) = 0;
	
		virtual bool setupInput(ofBaseSoundInput * soundInput, int numChannels, int sampleRate, int bufferSize, int nBuffers) = 0;
		virtual bool setupOutput(ofBaseSoundOutput * soundOutput, int numChannels, int sampleRate, int bufferSize, int nBuffers) = 0;
		
		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void close() = 0;

		virtual long unsigned long getTickCount() = 0;

		virtual void incrementTickCount() = 0;
};