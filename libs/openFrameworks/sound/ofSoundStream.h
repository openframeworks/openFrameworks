#pragma once

#include "ofConstants.h"
#include "ofBaseTypes.h"
#include "ofBaseApp.h"
#include "ofTypes.h"
#include "ofBaseSoundStream.h"

#ifdef OF_SOUNDSTREAM_RTAUDIO
	#include "ofRtAudioSoundStream.h"
	#define OF_SOUND_STREAM_TYPE ofRtAudioSoundStream
#elif defined(OF_SOUNDSTREAM_PORTAUDIO)
	#include "ofPASoundStream.h"
	#define OF_SOUND_STREAM_TYPE ofPASoundStream
#elif defined(OF_SOUNDSTREAM_ANDROID)
	#include "ofxAndroidSoundStream.h"
	#define OF_SOUND_STREAM_TYPE ofxAndroidSoundStream
#elif defined(OF_SOUNDSTREAM_IPHONE)
	#include "ofxiPhoneSoundStream.h"
	#define OF_SOUND_STREAM_TYPE ofxiPhoneSoundStream
#endif 

void ofSoundStreamSetup(int nOutputChannels, int nInputChannels, ofBaseApp * appPtr = NULL);
void ofSoundStreamSetup(int nOutputChannels, int nInputChannels, int sampleRate, int bufferSize, int nBuffers);
void ofSoundStreamSetup(int nOutputChannels, int nInputChannels, ofBaseApp * appPtr, int sampleRate, int bufferSize, int nBuffers);
void ofSoundStreamStop();
void ofSoundStreamStart();
void ofSoundStreamClose();
void ofSoundStreamListDevices();

class ofSoundStream{
	public:
		ofSoundStream();
		
		void setSoundStream(ofPtr<ofBaseSoundStream> soundStreamPtr);
		ofPtr<ofBaseSoundStream> getSoundStream();

		void listDevices();
	
		void setDeviceID(int deviceID);

		bool setup(ofBaseApp * app, int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers);
		void setInput(ofBaseSoundInput * soundInput);
		void setOutput(ofBaseSoundOutput * soundOutput);
		bool setup(int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers);
		
		void start();
		void stop();
		void close();
		
		long unsigned long getTickCount();
		
	protected:
		
		ofPtr<ofBaseSoundStream> soundStream;
};
