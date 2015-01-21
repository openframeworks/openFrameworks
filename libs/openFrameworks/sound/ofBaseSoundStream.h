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
	
		virtual std::vector<ofSoundDevice> getDeviceList() = 0;
		virtual void printDeviceList();
	
		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void close() = 0;

		virtual long unsigned long getTickCount() = 0;
		virtual int getNumInputChannels() = 0;
		virtual int getNumOutputChannels() = 0;
		virtual int getSampleRate() = 0;
		virtual int getBufferSize() = 0;
		virtual int getDeviceID() = 0;
	
		// deprecated
		virtual std::vector<ofSoundDevice> OF_DEPRECATED_MSG("Use printDeviceList instead", listDevices());
	
	protected:
		/// set tickCount and deviceID on the buffer
		void applySoundStreamOriginInfo( ofSoundBuffer& buffer );
};
