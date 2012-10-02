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


class ofSoundStream;

ofSoundStream* ofSoundStreamGetSystemStream();

void ofSoundStreamSetup(int nOutputChannels, int nInputChannels, ofBaseApp * appPtr = NULL);
void ofSoundStreamSetup(int nOutputChannels, int nInputChannels, int sampleRate, int bufferSize, int nBuffers);
void ofSoundStreamSetup(int nOutputChannels, int nInputChannels, ofBaseApp * appPtr, int sampleRate, int bufferSize, int nBuffers);
void ofSoundStreamStop();
void ofSoundStreamStart();
void ofSoundStreamClose();
void ofSoundStreamListDevices();

class ofSoundStream: public ofBaseSoundStream{
	public:
		ofSoundStream();
		
		void setSoundStream(ofPtr<ofBaseSoundStream> soundStreamPtr);
		ofPtr<ofBaseSoundStream> getSoundStream();

		void listDevices();
	
		void setDeviceID(int deviceID);

		bool isSetup();
		bool setupInput( int nChannels=2, int sampleRate=-1, int bufferSize=-1, int nBuffers=-1 );
		bool setupOutput( int nChannels=2, int sampleRate=-1, int bufferSize=-1, int nBuffers=-1 );

		bool setup(ofBaseApp * app, int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers);
		bool setup(int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers);

		/// set root input/output objects
		void setInput(ofBaseSoundInput * soundInput);
		void setOutput(ofBaseSoundOutput * soundOutput);
		
		void start();
		void stop();
		void close();
		
		long unsigned long getTickCount();
		int getNumInputChannels();
		int getNumOutputChannels();
		int getSampleRate();
		int getBufferSize();
		int getNumBuffers();
		
	protected:
		
		ofPtr<ofBaseSoundStream> soundStream;
};
