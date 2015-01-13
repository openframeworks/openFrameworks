#pragma once

#include <jni.h>
#include <queue>

#include "ofConstants.h"
#include "ofBaseSoundStream.h"
#include "ofxAndroidCircBuffer.h"

class ofxAndroidSoundStream : public ofBaseSoundStream{
	public:
		ofxAndroidSoundStream();
        ~ofxAndroidSoundStream();
		
		void listDevices();
		void setDeviceID(int deviceID);

		void setInput(ofBaseSoundInput * soundInput);
		void setOutput(ofBaseSoundOutput * soundOutput);
		bool setup(int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers);
		bool setup(ofBaseApp * app, int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers);
		
		void start();
		void stop();
		void close();
		
		long unsigned long getTickCount();		

        int getDeviceID(){return 0;}
		int getNumInputChannels();
		int getNumOutputChannels();
		int getSampleRate();
		int getBufferSize();

		int androidInputAudioCallback(JNIEnv*  env, jobject  thiz,jshortArray array, jint numChannels, jint bufferSize);
		int androidOutputAudioCallback(JNIEnv*  env, jobject  thiz,jshortArray array, jint numChannels, jint bufferSize);
		
		int getMinOutBufferSize(int samplerate, int nchannels);
		int getMinInBufferSize(int samplerate, int nchannels);

		bool isHeadPhonesConnected();

		ofEvent<bool> headphonesConnectedE;

	private:
		long unsigned long	tickCount;
		int			sampleRate;
		// pointers to OF audio callback classes
		ofBaseSoundInput *  soundInputPtr;
		ofBaseSoundOutput * soundOutputPtr;
		
		ofxAndroidCircBuffer<float> input_buffer;

		// 16-bits integers buffers used for Android PCM data
		short * out_buffer, * in_buffer;
		// 32-bits float buffers used by OF audio callbacks
		float * out_float_buffer, * in_float_buffer;
		// I/O buffers sizes/channels
		int outBufferSize, outChannels, inBufferSize, inChannels;
		//
		int  requestedBufferSize;
		int  totalOutRequestedBufferSize;
		int  totalInRequestedBufferSize;

		bool isPaused;

		friend void ofxAndroidSoundStreamPause();
		friend void ofxAndroidSoundStreamResume();
		void pause();
		void resume();
};
