#pragma once

#include "ofConstants.h"
#include "ofBaseTypes.h"

#ifdef OF_TARGET_IPHONE
	#error we need swappable sound stream api for iphone
#else
	#include "ofRtAudioSoundStream.h"
	#define OF_SOUND_STREAM_TYPE ofRtAudioSoundStream()
#endif 

class ofSoundStream{
	public:
		ofSoundStream();
		
		void setSoundStream(ofBaseSoundStream * soundStreamPtr);
		void listDevices();
	
		void setDeviceID(int deviceID);
		bool setup(ofSoundStreamMode soundStreamMode, ofBaseHasSoundStream * hasSoundStream, int numChannels, int sampleRate, int bufferSize, int nBuffers);
		
		void start();
		void stop();
		void close();
		
		long unsigned long getTickCount();
		
	protected:
		
		ofBaseSoundStream * soundStream;
};