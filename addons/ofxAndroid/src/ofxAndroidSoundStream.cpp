/*
 * ofxAndroidSoundStream.cpp
 *
 *  Created on: 04/07/2010
 *      Author: arturo
 */

#include "ofBaseApp.h"
#include "ofxAndroidSoundStream.h"
#include "ofUtils.h"
#include "ofxAndroidUtils.h"
#include "ofAppRunner.h"
#include <deque>
#include <set>
#include <jni.h>

static ofxAndroidSoundStream* instance=NULL;
static bool headphonesConnected;

ofxAndroidSoundStream::ofxAndroidSoundStream(){
	out_buffer=NULL;
	in_buffer=NULL;
	out_float_buffer=NULL;
	in_float_buffer=NULL;
	outBufferSize=0;
	outChannels=0;
	inBufferSize=0;
	inChannels=0;
	isPaused = false;
	soundInputPtr = NULL;
	soundOutputPtr = NULL;
	headphonesConnected = false;
}

ofxAndroidSoundStream::~ofxAndroidSoundStream(){
	if(instance==this){
		instance = NULL;
	}
}

void ofxAndroidSoundStream::listDevices(){

}

void ofxAndroidSoundStream::setDeviceID(int deviceID){

}

void ofxAndroidSoundStream::setInput(ofBaseSoundInput * _soundInput){
	soundInputPtr = _soundInput;
}

void ofxAndroidSoundStream::setOutput(ofBaseSoundOutput * _soundOutput){
	soundOutputPtr = _soundOutput;
}

bool ofxAndroidSoundStream::setup(int outChannels, int _inChannels, int _sampleRate, int bufferSize, int nBuffers){
	if(instance!=NULL && instance!=this){
		ofLog(OF_LOG_ERROR,"ofxAndroidSoundStream: error, only one instance allowed by now");
		return false;
	}


	int input_buffer_size = _inChannels*getMinInBufferSize(_sampleRate,_inChannels)* 2;
	input_buffer.setup(input_buffer_size,0);

	if(in_float_buffer){
		delete[] in_float_buffer;
	}
	in_float_buffer = new float[bufferSize*_inChannels];
	inBufferSize = bufferSize;
	inChannels   = _inChannels;

	sampleRate			=  _sampleRate;
	tickCount			=  0;
	requestedBufferSize =  bufferSize;
	totalOutRequestedBufferSize = bufferSize*outChannels;
	totalInRequestedBufferSize = bufferSize*inChannels;

	if(!ofGetJavaVMPtr()){
		ofLog(OF_LOG_ERROR,"ofSoundStreamSetup: Cannot find java virtual machine");
		return false;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return false;
	}
	jclass javaClass = env->FindClass("cc.openframeworks.OFAndroidSoundStream");

	if(javaClass==0){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroidSoundStream java class");
		return false;
	}

	jmethodID soundStreamSingleton = env->GetStaticMethodID(javaClass,"getInstance","()Lcc/openframeworks/OFAndroidSoundStream;");
	if(!soundStreamSingleton){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroidSoundStream singleton method");
		return false;
	}
	jobject javaObject = env->CallStaticObjectMethod(javaClass,soundStreamSingleton);
	jmethodID javaSetup = env->GetMethodID(javaClass,"setup","(IIIII)V");
	if(javaObject && javaSetup)
		env->CallVoidMethod(javaObject,javaSetup,outChannels,inChannels,sampleRate,bufferSize,nBuffers);
	else
		ofLog(OF_LOG_ERROR, "cannot get OFAndroidSoundStream instance or setup method");

	instance = this;
	return true;
}

bool ofxAndroidSoundStream::setup(ofBaseApp * app, int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers){
	if(inChannels>0)  setInput(app);
	if(outChannels>0) setOutput(app);

	return setup(outChannels,inChannels,sampleRate,bufferSize,nBuffers);
}

void ofxAndroidSoundStream::start(){
	setup(outChannels,inChannels,sampleRate,requestedBufferSize,1);
}

void ofxAndroidSoundStream::stop(){
	pause();
}

void ofxAndroidSoundStream::close(){
	pause();

	if(!ofGetJavaVMPtr()){
		ofLog(OF_LOG_ERROR,"ofSoundStreamSetup: Cannot find java virtual machine");
		return;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return;
	}
	jclass javaClass = env->FindClass("cc.openframeworks.OFAndroidSoundStream");

	if(javaClass==0){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroidSoundStream java class");
		return;
	}

	jmethodID soundStreamSingleton = env->GetStaticMethodID(javaClass,"getInstance","()Lcc/openframeworks/OFAndroidSoundStream;");
	if(!soundStreamSingleton){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroidSoundStream singleton method");
		return;
	}
	jobject javaObject = env->CallStaticObjectMethod(javaClass,soundStreamSingleton);
	jmethodID javaStop = env->GetMethodID(javaClass,"stop","()V");
	if(javaObject && javaStop)
		env->CallVoidMethod(javaObject,javaStop);
	else
		ofLog(OF_LOG_ERROR, "cannot get OFAndroidSoundStream instance or stop method");
}

long unsigned long ofxAndroidSoundStream::getTickCount(){
	return tickCount;
}

int ofxAndroidSoundStream::getNumInputChannels(){
	return inChannels;
}

int ofxAndroidSoundStream::getNumOutputChannels(){
	return outChannels;
}


void ofxAndroidSoundStream::pause(){
	ofLogVerbose("ofxAndroidSoundStream") << "pause: releasing buffers";
	if(in_buffer)
		ofGetJNIEnv()->ReleasePrimitiveArrayCritical(jInArray,in_buffer,0);
	if(out_buffer)
		ofGetJNIEnv()->ReleasePrimitiveArrayCritical(jOutArray,out_buffer,0);
	in_buffer = NULL;
	out_buffer = NULL;
	isPaused = true;
}

void ofxAndroidSoundStream::resume(){
	isPaused = false;
}


static const float conv_factor = 1/32767.5f;

int ofxAndroidSoundStream::androidInputAudioCallback(JNIEnv*  env, jobject  thiz,jshortArray array, jint numChannels, jint bufferSize){
	//ofLogError("ofxAndroidSoundStream") <<  "input callback" << bufferSize;

	if(!soundInputPtr || isPaused) return 0;

	if(!in_buffer || !in_float_buffer || numChannels!=inChannels || bufferSize!=inBufferSize){
		if(in_buffer){
			env->ReleasePrimitiveArrayCritical(array,in_buffer,0);
			jInArray = 0;
		}
		in_buffer = (short*)env->GetPrimitiveArrayCritical(array, NULL);
		if(!in_buffer) return 1;
		jInArray = array;
	}


	for(int i=0;i<bufferSize*numChannels;i++){
		//in_float_buffer[i] = (float(in_buffer[i]) + 0.5) * conv_factor;

		input_buffer.write((float(in_buffer[i]) + 0.5) * conv_factor);
	}


	return 0;
}

int ofxAndroidSoundStream::androidOutputAudioCallback(JNIEnv*  env, jobject  thiz,jshortArray array, jint numChannels, jint bufferSize){

	if(!soundOutputPtr || isPaused) return 0;

	if(!out_buffer || !out_float_buffer || numChannels!=outChannels || bufferSize!=outBufferSize){
		if(out_buffer){
			env->ReleasePrimitiveArrayCritical(array,out_buffer,0);
			jOutArray = 0;
		}
		out_buffer = (short*)env->GetPrimitiveArrayCritical(array, NULL);
		if(!out_buffer) return 1;
		jOutArray = array;

		if(out_float_buffer) delete[] out_float_buffer;
		out_float_buffer = new float[bufferSize*numChannels];
		outBufferSize = bufferSize;
		outChannels   = numChannels;

		ofLogNotice("ofxAndroidSoundStream") << "setting out buffers frames: " << bufferSize;
	}


	if(inChannels>0){
		for(int i=0;i<bufferSize*inChannels;i++){
			in_float_buffer[i] = input_buffer.read(0);
		}
		soundInputPtr->audioIn(in_float_buffer,bufferSize,inChannels,0,tickCount);
	}

	soundOutputPtr->audioOut(out_float_buffer,bufferSize,numChannels,0,tickCount);

	for(int i=0;i<bufferSize*numChannels ;i++){
		float tempf = (out_float_buffer[i] * 32767.5f) - 0.5;
		out_buffer[i] = tempf;//lrintf( tempf - 0.5 );
	}
	tickCount++;

	/*int nBlocks = bufferSize/requestedBufferSize;
	//ofLogWarning("ofxAndroidSoundStream") << "total blocs: " << nBlocks;

	if (numChannels > 0) {
		float * out_buffer_ptr = out_float_buffer;

		//memset( out_float_buffer, 0, sizeof(float)*bufferSize*numChannels );
		for(int t = 0;t<nBlocks;t++){
			ofLogWarning("ofxAndroidSoundStream") << "block: " << t;
			tickCount++;
			soundOutputPtr->audioOut(out_buffer_ptr,requestedBufferSize,numChannels,tickCount);

			out_buffer_ptr+=totalOutRequestedBufferSize;
		}

		//ofLogWarning("ofxAndroidSoundStream") << "translating float to short";
		for(int i=0;i<bufferSize*numChannels ;i++){
			float tempf = (out_float_buffer[i] * 32767.5f) - 0.5;
			out_buffer[i]=tempf;//lrintf( tempf - 0.5 );
		}
	}*/

	return 0;
}

int ofxAndroidSoundStream::getMinOutBufferSize(int samplerate, int nchannels){
	jclass javaClass = ofGetJNIEnv()->FindClass("cc.openframeworks.OFAndroidSoundStream");

	if(javaClass==0){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroidSoundStream java class");
		return false;
	}

	jmethodID getMinBuffSize = ofGetJNIEnv()->GetStaticMethodID(javaClass,"getMinOutBufferSize","(II)I");
	if(!getMinBuffSize){
		ofLog(OF_LOG_ERROR,"cannot find getMinOutBufferSize method");
		return false;
	}
	int minBuff = ofGetJNIEnv()->CallStaticIntMethod(javaClass,getMinBuffSize,samplerate,nchannels);
	return minBuff;
}

int ofxAndroidSoundStream::getMinInBufferSize(int samplerate, int nchannels){
	jclass javaClass = ofGetJNIEnv()->FindClass("cc.openframeworks.OFAndroidSoundStream");

	if(javaClass==0){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroidSoundStream java class");
		return false;
	}

	jmethodID getMinBuffSize = ofGetJNIEnv()->GetStaticMethodID(javaClass,"getMinInBufferSize","(II)I");
	if(!getMinBuffSize){
		ofLog(OF_LOG_ERROR,"cannot find getMinInBufferSize method");
		return false;
	}
	return ofGetJNIEnv()->CallStaticIntMethod(javaClass,getMinBuffSize,samplerate,nchannels);
}

bool ofxAndroidSoundStream::isHeadPhonesConnected(){
	return headphonesConnected;
}

void ofxAndroidSoundStreamPause(){
	if(instance){
		instance->pause();
	}
}

void ofxAndroidSoundStreamResume(){
	if(instance){
		instance->resume();
	}
}

extern "C"{

jint
Java_cc_openframeworks_OFAndroidSoundStream_audioOut(JNIEnv*  env, jobject  thiz, jshortArray array, jint numChannels, jint bufferSize){
	if(instance){
		return instance->androidOutputAudioCallback(env,thiz,array,numChannels,bufferSize);
	}
	return 0;
}


jint
Java_cc_openframeworks_OFAndroidSoundStream_audioIn(JNIEnv*  env, jobject  thiz, jshortArray array, jint numChannels, jint bufferSize){
	if(instance){
		return instance->androidInputAudioCallback(env,thiz,array,numChannels,bufferSize);
	}
	return 0;
}

void Java_cc_openframeworks_OFAndroidSoundStream_headphonesConnected(JNIEnv*  env, jobject  thiz, jboolean connected){
	headphonesConnected = connected;
	if(instance) ofNotifyEvent(instance->headphonesConnectedE,headphonesConnected);
}
}
