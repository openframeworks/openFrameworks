#pragma once
#include "ofConstants.h"


class ofSoundBuffer;

class ofBaseSoundInput;
class ofBaseSoundOutput;

class ofBaseSoundStream{
	public:
		virtual ~ofBaseSoundStream(){}
		
		virtual void listDevices() = 0;
		virtual void setDeviceID(int deviceID) = 0;
		virtual bool setup(int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers)=0;
		virtual bool setup(ofBaseApp * app, int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers)=0;
		virtual void setInput(ofBaseSoundInput * soundInput) = 0;
		virtual void setOutput(ofBaseSoundOutput * soundOutput) = 0;
        virtual ofBaseSoundInput * getInput() = 0;
        virtual ofBaseSoundOutput * getOutput() = 0;
		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void close() = 0;

		virtual long unsigned long getTickCount() = 0;
		virtual int getNumInputChannels() = 0;
		virtual int getNumOutputChannels() = 0;
		virtual int getSampleRate() = 0;
		virtual int getBufferSize() = 0;
		virtual int getDeviceID() = 0;
	
protected:
	
	/// set tickCount and deviceID on the buffer
	void applySoundStreamOriginInfo( ofSoundBuffer* buffer );
};
