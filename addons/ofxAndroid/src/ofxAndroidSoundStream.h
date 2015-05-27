#pragma once

#include <jni.h>

#include "ofConstants.h"
#include "ofBaseSoundStream.h"
#include "ofxAndroidCircBuffer.h"
#include "ofSoundBuffer.h"

class ofxAndroidSoundStream : public ofBaseSoundStream{
	public:
		ofxAndroidSoundStream();
        ~ofxAndroidSoundStream();
		
		std::vector<ofSoundDevice> getDeviceList() const;
		void setDeviceID(int deviceID);

		void setInput(ofBaseSoundInput * soundInput);
		void setOutput(ofBaseSoundOutput * soundOutput);
		bool setup(int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers);
		bool setup(ofBaseApp * app, int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers);
		
		void start();
		void stop();
		void close();
		
		long unsigned long getTickCount() const;

        int getDeviceID() const{return 0;}
		int getNumInputChannels() const;
		int getNumOutputChannels() const;
		int getSampleRate() const;
		int getBufferSize() const;

		int androidInputAudioCallback(JNIEnv*  env, jobject  thiz,jshortArray array, jint numChannels, jint bufferSize);
		int androidOutputAudioCallback(JNIEnv*  env, jobject  thiz,jshortArray array, jint numChannels, jint bufferSize);
		
		int getMinOutBufferSize(int samplerate, int nchannels) const;
		int getMinInBufferSize(int samplerate, int nchannels) const;

		bool isHeadPhonesConnected() const;

		ofEvent<bool> headphonesConnectedE;

	private:
		long unsigned long	tickCount;
		// pointers to OF audio callback classes
		ofBaseSoundInput *  soundInputPtr;
		ofBaseSoundOutput * soundOutputPtr;
		
		ofxAndroidCircBuffer<float> input_buffer;

		// 16-bits integers buffers used for Android PCM data
		short * out_buffer, * in_buffer;
		// 32-bits float buffers used by OF audio callbacks
		ofSoundBuffer in_float_buffer, out_float_buffer;
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
