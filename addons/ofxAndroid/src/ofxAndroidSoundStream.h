#pragma once

#include <jni.h>

#include "ofConstants.h"
#include "ofSoundBaseTypes.h"
#include "ofxAndroidCircBuffer.h"
#include "ofSoundBuffer.h"
#include "ofEvents.h"

class ofxAndroidSoundStream : public ofBaseSoundStream{
	public:
		ofxAndroidSoundStream();
        ~ofxAndroidSoundStream();

		std::vector<ofSoundDevice> getDeviceList(ofSoundDevice::Api api) const;
		void setDeviceID(int deviceID);

		void setInput(ofBaseSoundInput * soundInput);
		void setOutput(ofBaseSoundOutput * soundOutput);
		bool setup(const ofSoundStreamSettings & settings);
		
		void start();
		void stop();
		void close();

		uint64_t getTickCount() const;

		ofSoundDevice getInDevice() const{ return ofSoundDevice(); }
		ofSoundDevice getOutDevice() const{ return ofSoundDevice(); }
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
		ofSoundStreamSettings settings;
		
		ofxAndroidCircBuffer<float> input_buffer;

		// 16-bits integers buffers used for Android PCM data
		short * out_buffer, * in_buffer;
		// 32-bits float buffers used by OF audio callbacks
		ofSoundBuffer in_float_buffer, out_float_buffer;
		//
		int  totalOutRequestedBufferSize;
		int  totalInRequestedBufferSize;

		bool isPaused;

		friend void ofxAndroidSoundStreamPause();
		friend void ofxAndroidSoundStreamResume();
		void pause();
		void resume();
};
