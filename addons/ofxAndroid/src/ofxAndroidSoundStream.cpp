/*
 * ofxAndroidSoundStream.cpp
 *
 *  Created on: 04/07/2010
 *      Author: arturo
 */

#include "ofBaseApp.h"
#include "ofxAndroidSoundStream.h"
#include "ofSoundStream.h"
#include "ofUtils.h"
#include "ofxAndroidUtils.h"
#include "ofAppRunner.h"
#include "ofLog.h"
#include <deque>
#include <set>
#include <jni.h>

using namespace std;

// Global pointer used to implement the singletomn pattern for ofxAndroidSoundStream class
static ofxAndroidSoundStream* instance = NULL;
static bool headphonesConnected = false;

ofxAndroidSoundStream::ofxAndroidSoundStream(){
	out_buffer = NULL;
	in_buffer = NULL;

	isPaused = false;
	totalOutRequestedBufferSize = totalInRequestedBufferSize = 0;
	tickCount = 0;
	headphonesConnected = false;

	ofAddListener(ofxAndroidEvents().pause,this,&ofxAndroidSoundStream::pause);
	ofAddListener(ofxAndroidEvents().resume,this,&ofxAndroidSoundStream::resume);
}

ofxAndroidSoundStream::~ofxAndroidSoundStream(){
	if(instance==this){
		instance = NULL;
	}

	ofRemoveListener(ofxAndroidEvents().pause,this,&ofxAndroidSoundStream::pause);
	ofRemoveListener(ofxAndroidEvents().resume,this,&ofxAndroidSoundStream::resume);
}

std::vector<ofSoundDevice> ofxAndroidSoundStream::getDeviceList(ofSoundDevice::Api api) const{
    ofLogWarning("ofxAndroidSoundStream") << "getDeviceList() isn't implemented on android";
    return vector<ofSoundDevice>();
}

void ofxAndroidSoundStream::setDeviceID(int deviceID){

}

void ofxAndroidSoundStream::setInput(ofBaseSoundInput * _soundInput){
	settings.setInListener(_soundInput);
}

void ofxAndroidSoundStream::setOutput(ofBaseSoundOutput * _soundOutput){
    settings.setOutListener(_soundOutput);
}

bool ofxAndroidSoundStream::setup(const ofSoundStreamSettings & settings){
	if(instance!=NULL && instance!=this){
		ofLogError("ofxAndroidSoundStream") << "setup(): multiple instances detected, only one instance allowed";
		return false;
	}

	// Find the minimum input buffer size allowed by the Android device
	int input_buffer_size = settings.numInputChannels*getMinInBufferSize(settings.sampleRate,settings.numInputChannels) * 2;
	// setup size of input circular-buffer
	input_buffer.setup(input_buffer_size,0);
	
	// deallocate and reallocate if setup() is called more than once
	in_float_buffer.allocate(settings.bufferSize,settings.numInputChannels);
	in_float_buffer.setSampleRate(settings.sampleRate);
	out_float_buffer.allocate(settings.bufferSize,settings.numOutputChannels);
	out_float_buffer.setSampleRate(settings.sampleRate);

	tickCount = 0;

	totalOutRequestedBufferSize = settings.bufferSize*settings.numOutputChannels;
	totalInRequestedBufferSize = settings.bufferSize*settings.numInputChannels;
    this->settings = settings;

	// JNI: Try to find and call OFAndroidSoundStream.getInstance().setup(outChannels,inChannels,sampleRate,bufferSize,nBuffers)
	if(!ofGetJavaVMPtr()){
		ofLogError("ofxAndroidSoundStream") << "setup(): couldn't find java virtual machine";
		return false;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError("ofxAndroidSoundStream") << "setup(): couldn't get environment using GetEnv()";
		return false;
	}
	jclass javaClass = env->FindClass("cc/openframeworks/OFAndroidSoundStream");

	if(javaClass==0){
		ofLogError("ofxAndroidSoundStream") << "setup(): couldn't find OFAndroidSoundStream java class";
		return false;
	}

	jmethodID soundStreamSingleton = env->GetStaticMethodID(javaClass,"getInstance","()Lcc/openframeworks/OFAndroidSoundStream;");
	if(!soundStreamSingleton){
		ofLogError("ofxAndroidSoundStream") << "setup(): couldn't find OFAndroidSoundStream singleton method";
		return false;
	}
	jobject javaObject = env->CallStaticObjectMethod(javaClass,soundStreamSingleton);
	jmethodID javaSetup = env->GetMethodID(javaClass,"setup","(IIIII)V");
	// call setup()
	if(javaObject && javaSetup){
		env->CallVoidMethod(javaObject,javaSetup,settings.numOutputChannels,settings.numInputChannels,settings.sampleRate,settings.bufferSize,settings.numBuffers);
	}else{
		ofLogError("ofxAndroidSoundStream") << "setup(): couldn't get OFAndroidSoundStream instance or setup method";
	}
	// Store instance pointer to ofxAndroidSoundStream (singleton pattern)
	instance = this;
	isPaused = false;

	return true;
}

void ofxAndroidSoundStream::start(){
	if(isPaused){
		resume();
	}else{
		setup(settings);
	}
}

void ofxAndroidSoundStream::stop(){
	pause();
}

void ofxAndroidSoundStream::close(){
	pause();

	// JNI: Try to find and call OFAndroidSoundStream.getInstance().stop()
	if(!ofGetJavaVMPtr()){
		ofLogError("ofxAndroidSoundStream") << "close(): couldn't find java virtual machine";
		return;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError("ofxAndroidSoundStream") << "close(): couldn't get environment using GetEnv()";
		return;
	}
	jclass javaClass = env->FindClass("cc/openframeworks/OFAndroidSoundStream");

	if(javaClass==0){
		ofLogError("ofxAndroidSoundStream") << "close(): couldn't find OFAndroidSoundStream java class";
		return;
	}

	jmethodID soundStreamSingleton = env->GetStaticMethodID(javaClass,"getInstance","()Lcc/openframeworks/OFAndroidSoundStream;");
	if(!soundStreamSingleton){
		ofLogError("ofxAndroidSoundStream") << "close(): couldn't find OFAndroidSoundStream singleton method";
		return;
	}
	jobject javaObject = env->CallStaticObjectMethod(javaClass,soundStreamSingleton);
	jmethodID javaStop = env->GetMethodID(javaClass,"stop","()V");
	// call stop()
	if(javaObject && javaStop)
		env->CallVoidMethod(javaObject,javaStop);
	else
		ofLogError("ofxAndroidSoundStream") << "close(): couldn't get OFAndroidSoundStream instance or stop method";
}

uint64_t ofxAndroidSoundStream::getTickCount() const{
	return tickCount;
}

int ofxAndroidSoundStream::getNumInputChannels() const{
	return in_float_buffer.getNumChannels();
}

int ofxAndroidSoundStream::getNumOutputChannels() const{
	return out_float_buffer.getNumChannels();
}

int ofxAndroidSoundStream::getSampleRate() const{
	return out_float_buffer.getSampleRate();
}

int ofxAndroidSoundStream::getBufferSize() const{
	return out_float_buffer.getNumFrames();
}

void ofxAndroidSoundStream::pause(){
	isPaused = true;
}

void ofxAndroidSoundStream::resume(){
	isPaused = false;
}


static const float conv_factor = 1/32767.5f;

int ofxAndroidSoundStream::androidInputAudioCallback(JNIEnv*  env, jobject  thiz,jshortArray array, jint numChannels, jint bufferSize){

	if(!settings.inCallback || isPaused){
		return 0;
	}

	// /* comment this below out ?
	// --- Handle INPUT buffer size changes ---
	// if our 'out_float_buffer' (used by the OF audio output callback) is not allocated
	// OR the number of output channels has changed
	// OR the output buffer size has changed
	// then: free/reallocate 'out_float_buffer' accordingly
	if(in_float_buffer.size() != numChannels * bufferSize){
		in_float_buffer.allocate(bufferSize,numChannels);
		ofLogNotice("ofxAndroidSoundStream") << "setting input buffers frames to: " << bufferSize;		
	} // */

	// IMPORTANT: Critical buffers must be Acquired / Released ASAP:
	// http://download.oracle.com/javase/1.3/docs/guide/jni/jni-12.html#GetPrimitiveArrayCritical

    // 1) Get critical JNI buffer access
	in_buffer = (short*)env->GetPrimitiveArrayCritical(array, NULL);
	if(in_buffer == NULL) return 1; // this would imply 'Out of memory' exception

	// 2) Perform input buffer copy (write into OpenFrameworks circular buffer)
	if(out_float_buffer.getNumChannels()>0){
		for(int i=0;i<bufferSize*numChannels;i++){
			input_buffer.write((float(in_buffer[i]) + 0.5f) * conv_factor);
		}
	}else{
		in_float_buffer.copyFrom(in_buffer,bufferSize,numChannels,in_float_buffer.getSampleRate());
		in_float_buffer.setTickCount(tickCount);
        settings.inCallback(in_float_buffer);
	}

	// 3) Release critical JNI
	env->ReleasePrimitiveArrayCritical(array, in_buffer,0);

	return 0; // all ok
}

int ofxAndroidSoundStream::androidOutputAudioCallback(JNIEnv*  env, jobject  thiz,jshortArray array, jint numChannels, jint bufferSize){

	if((!settings.outCallback && !settings.inCallback) || isPaused){
        return 0;
    }

	// --- Handle OUTPUT buffer size changes ---
	// if our 'out_float_buffer' (used by the OF audio output callback) is not allocated
	// OR the number of output channels has changed
	// OR the output buffer size has changed
	// then: free/reallocate 'out_float_buffer' accordingly
	if(out_float_buffer.size()!=bufferSize*numChannels){
		out_float_buffer.allocate(bufferSize,numChannels);
		ofLogNotice("ofxAndroidSoundStream") << "setting out buffers frames to: " << bufferSize;		
	}

   	// First, the "sound input" circular buffer is handled into our "sound output" function.
	//  (minimizing latency in full-duplex, see: http://www.portaudio.com/docs/v19-doxydocs/pa__process_8h.html )
	if(settings.inCallback && in_float_buffer.getNumChannels()>0){
		for(size_t i=0;i<in_float_buffer.size();i++){
			in_float_buffer[i] = input_buffer.read(0);
		}
		in_float_buffer.setTickCount(tickCount);
        settings.inCallback(in_float_buffer);
	}

	// 1) Get critical JNI buffer access
	if(settings.outCallback){
		out_buffer = (short*)env->GetPrimitiveArrayCritical(array, NULL);
		if(!out_buffer){
            return 1;
        }

		// Call the ofApp soundOutput() Callback so that 'out_float_buffer' gets filled
		out_float_buffer.set(0);
		out_float_buffer.setTickCount(tickCount);
        settings.outCallback(out_float_buffer);
		out_float_buffer.toShortPCM(out_buffer);

		// 3) Release critical JNI: now that the JNI output buffer has been populated, release it ASAP
		env->ReleasePrimitiveArrayCritical(array,out_buffer,0);
	}

	tickCount++;

	return 0;
}

int ofxAndroidSoundStream::getMinOutBufferSize(int samplerate, int nchannels) const{
	jclass javaClass = ofGetJNIEnv()->FindClass("cc/openframeworks/OFAndroidSoundStream");

	if(javaClass==0){
		ofLogError("ofxAndroidSoundStream") << "getMinOutBufferSize(): couldn't find OFAndroidSoundStream java class";
		return false;
	}

	jmethodID getMinBuffSize = ofGetJNIEnv()->GetStaticMethodID(javaClass,"getMinOutBufferSize","(II)I");
	if(!getMinBuffSize){
		ofLogError("ofxAndroidSoundStream") << "getMinOutBufferSize(): couldn't find getMinOutBufferSize method";
		return false;
	}
	int minBuff = ofGetJNIEnv()->CallStaticIntMethod(javaClass,getMinBuffSize,samplerate,nchannels);
	return minBuff;
}

int ofxAndroidSoundStream::getMinInBufferSize(int samplerate, int nchannels) const{
	jclass javaClass = ofGetJNIEnv()->FindClass("cc/openframeworks/OFAndroidSoundStream");

	if(javaClass==0){
		ofLogError("ofxAndroidSoundStream") << "getMinInBufferSize(): couldn't find OFAndroidSoundStream java class";
		return false;
	}

	jmethodID getMinBuffSize = ofGetJNIEnv()->GetStaticMethodID(javaClass,"getMinInBufferSize","(II)I");
	if(!getMinBuffSize){
		ofLogError("ofxAndroidSoundStream") << "getMinInBufferSize(): couldn't find getMinInBufferSize method";
		return false;
	}
	return ofGetJNIEnv()->CallStaticIntMethod(javaClass,getMinBuffSize,samplerate,nchannels);
}

bool ofxAndroidSoundStream::isHeadPhonesConnected() const{
	return headphonesConnected;
}

extern "C"{

jint
Java_cc_openframeworks_OFAndroidSoundStream_audioOut(JNIEnv*  env, jobject  thiz, jshortArray array, jint numChannels, jint bufferSize){
	if(instance){
		return instance->androidOutputAudioCallback(env,thiz,array,numChannels,bufferSize);
	}else{
		ofLogError("Java_cc_openframeworks_OFAndroidSoundStream_audioOut") << "No instance";
	}
	return 0;
}


jint
Java_cc_openframeworks_OFAndroidSoundStream_audioIn(JNIEnv*  env, jobject  thiz, jshortArray array, jint numChannels, jint bufferSize){
	if(instance){
		return instance->androidInputAudioCallback(env,thiz,array,numChannels,bufferSize);
	}else{
		ofLogError("Java_cc_openframeworks_OFAndroidSoundStream_audioIn") << "No instance";
	}
	return 0;
}

void Java_cc_openframeworks_OFAndroidSoundStream_headphonesConnected(JNIEnv*  env, jobject  thiz, jboolean connected){
	headphonesConnected = connected;
	if(instance) ofNotifyEvent(instance->headphonesConnectedE,headphonesConnected);
}
}
