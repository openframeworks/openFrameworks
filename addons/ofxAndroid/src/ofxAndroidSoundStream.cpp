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
#include <deque>
#include <set>

static ofxAndroidSoundStream* instance=NULL;



ofxAndroidSoundStream::ofxAndroidSoundStream(){
	out_buffer=NULL;
	in_buffer=NULL;
	out_float_buffer=NULL;
	in_float_buffer=NULL;
	outBufferSize=0;
	outChannels=0;
	inBufferSize=0;
	inChannels=0;
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

bool ofxAndroidSoundStream::setup(int outChannels, int inChannels, int _sampleRate, int bufferSize, int nBuffers){
	if(instance!=NULL){
		ofLog(OF_LOG_ERROR,"ofxAndroidSoundStream: error, only one instance allowed by now");
		return false;
	}

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
}

bool ofxAndroidSoundStream::setup(ofBaseApp * app, int outChannels, int inChannels, int sampleRate, int bufferSize, int nBuffers){
	setInput(app);
	setOutput(app);
	return setup(outChannels,inChannels,sampleRate,bufferSize,nBuffers);
}

void ofxAndroidSoundStream::start(){

}

void ofxAndroidSoundStream::stop(){

}

void ofxAndroidSoundStream::close(){

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
	if(in_buffer)
		ofGetJNIEnv()->ReleasePrimitiveArrayCritical(jInArray,in_buffer,0);
	if(out_buffer)
		ofGetJNIEnv()->ReleasePrimitiveArrayCritical(jOutArray,out_buffer,0);
	in_buffer = NULL;
	out_buffer = NULL;
}


static float conv_factor = 1/32767.5f;

int ofxAndroidSoundStream::androidInputAudioCallback(JNIEnv*  env, jobject  thiz,jshortArray array, jint numChannels, jint bufferSize){
	if(!in_float_buffer || numChannels!=inChannels || bufferSize!=inBufferSize){
		if(in_buffer)
			env->ReleasePrimitiveArrayCritical(array,in_buffer,0);
		in_buffer = (short*)env->GetPrimitiveArrayCritical(array, NULL);
		if(!in_buffer) return 1;

		if(in_float_buffer) delete[] in_float_buffer;
		in_float_buffer = new float[bufferSize*numChannels];
		inBufferSize = bufferSize;
		inChannels   = numChannels;
	}
	if(soundInputPtr){
		for(int i=0;i<bufferSize*numChannels;i++){
			in_float_buffer[i] = (float(in_buffer[i]) + 0.5) * conv_factor;
		}
		soundInputPtr->audioIn(in_float_buffer,bufferSize,numChannels,tickCount);
	}

	return 0;
}

int ofxAndroidSoundStream::androidOutputAudioCallback(JNIEnv*  env, jobject  thiz,jshortArray array, jint numChannels, jint bufferSize){

	if(!out_buffer || !out_float_buffer || numChannels!=outChannels || bufferSize!=outBufferSize){
		if(out_buffer)
			env->ReleasePrimitiveArrayCritical(array,out_buffer,0);
		out_buffer = (short*)env->GetPrimitiveArrayCritical(array, NULL);
		if(!out_buffer) return 1;
		if(out_float_buffer) delete[] out_float_buffer;
		out_float_buffer = new float[bufferSize*numChannels];
		outBufferSize = bufferSize;
		outChannels   = numChannels;
	}

	if (numChannels > 0) {
		float * out_buffer_ptr = out_float_buffer;
		memset( out_float_buffer, 0, sizeof(float)*bufferSize*numChannels );
		for(int t=0;t<bufferSize/requestedBufferSize;t++){
			tickCount++;
			if(soundOutputPtr){
				soundOutputPtr->audioOut(out_buffer_ptr,requestedBufferSize,numChannels,tickCount);
			}
			out_buffer_ptr+=totalOutRequestedBufferSize;
		}
		//time_one_frame = ofGetSystemTime();
		for(int i=0;i<bufferSize*numChannels ;i++){
			short tempf = (out_float_buffer[i] * 32767.5f) - 0.5;
			out_buffer[i]=tempf;//lrintf( tempf - 0.5 );
		}
	}

	return 0;
}

void ofxAndroidSoundStreamPause(){
	if(instance){
		instance->pause();
	}
}

extern "C"{

/*
void
Java_cc_openframeworks_OFAndroidSoundStream_initAudioOutput(JNIEnv*  env, jobject  thiz, jobject javaSoundStream){
	jclass javaClass = env->FindClass("cc.openframeworks.OFAndroidSoundStream");
	if(javaClass!=0){
		javaObject = javaSoundStream;
		javaSetup = env->GetMethodID(javaClass,"setup","(IIIII)V");
	}
	javaEnv = env;
}*/

/*static int time_one_frame = 0;
static int acc_time = 0;
static int num_frames = 0;
static int time_prev_out = 0;*/

jint
Java_cc_openframeworks_OFAndroidSoundStream_audioOut(JNIEnv*  env, jobject  thiz, jshortArray array, jint numChannels, jint bufferSize){
	if(instance){
		instance->androidOutputAudioCallback(env,thiz,array,numChannels,bufferSize);
	}
}


jint
Java_cc_openframeworks_OFAndroidSoundStream_audioReceived(JNIEnv*  env, jobject  thiz, jshortArray array, jint numChannels, jint bufferSize){
	if(instance){
		instance->androidInputAudioCallback(env,thiz,array,numChannels,bufferSize);
	}
}
}
