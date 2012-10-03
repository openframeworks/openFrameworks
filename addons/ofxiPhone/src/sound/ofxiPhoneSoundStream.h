#pragma once

#include "ofConstants.h"

#include "ofBaseSoundStream.h"
#include "ofTypes.h"


#include <AudioToolbox/AudioToolbox.h>

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
		/// the max buffersize is 4096 
		bool setup(int outChannels, int inChannels, int sampleRate, int nFramesPerBuffer, int nBuffers);
		bool setup(ofBaseApp * app, int outChannels, int inChannels, int sampleRate, int nFramesPerBuffer, int nBuffers);
		
		void start();
		void stop();
		void close();
		
		// not implemented on iOS, always returns 0
		long unsigned long getTickCount();
	
		int getNumInputChannels();
		int getNumOutputChannels();
		int getSampleRate();
		int getBufferSize();
		int getNumBuffers() { return nBuffers; }
		
	private:
		
		long unsigned long	tickCount;
		int					nInputChannels;
		int					nOutputChannels;
		int					sampleRate;
		int                 nFramesPerBuffer;
		int 				nBuffers;
	
		ofBaseSoundInput *			soundInputPtr;
		ofBaseSoundOutput *			soundOutputPtr;

		AudioStreamBasicDescription			format, audioFormat;
		AudioUnit							audioUnit		= NULL;
		AudioBufferList						inputBufferList;		// input buffer

		// flags to determine whether audio*BuffersChanged methods should be called on the soundInputPtr/soundOutputPtr
		bool				newBuffersNeededForInput;
		bool				newBuffersNeededForOutput;

		static OSStatus recordingCallback(void *inRefCon,
									  AudioUnitRenderActionFlags *ioActionFlags,
									  const AudioTimeStamp *inTimeStamp,
									  UInt32 inBusNumber,
									  UInt32 inNumberFrames,
									  AudioBufferList *ioData);
	
		static OSStatus playbackCallback(void *inRefCon, 
								   AudioUnitRenderActionFlags *ioActionFlags,
								   const AudioTimeStamp *inTimeStamp,
								   UInt32 inBusNumber,
								   UInt32 inNumberFrames,
									 AudioBufferList *ioData);


};


