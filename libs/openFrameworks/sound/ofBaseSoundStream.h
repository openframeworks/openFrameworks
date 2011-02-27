#pragma once
#include "ofConstants.h"

class ofBaseHasSoundStream;

class ofBaseSoundStream{
	public:
		
		virtual void listDevices() = 0;
		virtual void setDeviceID(int deviceID) = 0;
		virtual bool setup(ofSoundStreamMode soundStreamMode, ofBaseHasSoundStream * hasSoundStream, int numChannels, int sampleRate, int bufferSize, int nBuffers) = 0;
		
		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void close() = 0;

		virtual long unsigned long getTickCount() = 0;

};