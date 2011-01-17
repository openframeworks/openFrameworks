/*
 * ofxAndroidSoundStream.cpp
 *
 *  Created on: 04/07/2010
 *      Author: arturo
 */

#include <jni.h>
#include "ofBaseApp.h"
#include "ofSoundStream.h"
#include "ofUtils.h"
#include "ofxAndroidUtils.h"

static short * out_buffer, * in_buffer;
static float * out_float_buffer=NULL, * in_float_buffer=NULL;
static int outBufferSize=0, outChannels=0, inBufferSize=0, inChannels=0;
static ofBaseApp * OFApp;



void ofSoundStreamSetup(int nOutputChannels, int nInputChannels, ofBaseApp * OFSA, int sampleRate, int bufferSize, int nBuffers){
	OFApp = OFSA;
	if(!ofGetJavaVMPtr()){
		ofLog(OF_LOG_ERROR,"ofSoundStreamSetup: Cannot find java virtual machine");
		return;
	}
	JNIEnv *env;
	if (ofGetJavaVMPtr()->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
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
	jmethodID javaSetup = env->GetMethodID(javaClass,"setup","(IIIII)V");
	if(javaObject && javaSetup)
		env->CallVoidMethod(javaObject,javaSetup,nOutputChannels,nInputChannels,sampleRate,bufferSize,nBuffers);
	else
		ofLog(OF_LOG_ERROR, "cannot get OFAndroidSoundStream instance or setup method");
}

void ofSoundStreamSetup(int nOutputChannels, int nInputChannels, ofBaseApp * OFSA){
	ofSoundStreamSetup(nOutputChannels,nInputChannels,OFSA,44100,256,2);
}

void ofSoundStreamSetup(int nOutputChannels, int nInputChannels, int sampleRate, int bufferSize, int nBuffers){
	ofSoundStreamSetup(nOutputChannels,nInputChannels,NULL,sampleRate,bufferSize,nBuffers);
}

void ofSoundStreamStop(){

}

void ofSoundStreamStart(){

}

void ofSoundStreamClose(){

}

void ofSoundStreamListDevices(){

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
Java_cc_openframeworks_OFAndroidSoundStream_audioRequested(JNIEnv*  env, jobject  thiz, jshortArray array, jint numChannels, jint bufferSize){
	if(!out_float_buffer || numChannels!=outChannels || bufferSize!=outBufferSize){
		if(out_float_buffer) delete[] out_float_buffer;
		out_float_buffer = new float[bufferSize*numChannels];
		outBufferSize = bufferSize;
		outChannels   = numChannels;
	}
	if(OFApp){
		out_buffer = (short*)env->GetPrimitiveArrayCritical(array, NULL);
		if(!out_buffer) return 1;
		OFApp->audioRequested(out_float_buffer,bufferSize,numChannels);

		//time_one_frame = ofGetSystemTime();
		for(int i=0;i<bufferSize*numChannels;i++){
			short tempf = (out_float_buffer[i] * 32767.5f) - 0.5;
			out_buffer[i]=tempf;//lrintf( tempf - 0.5 );
		}
		/*acc_time += ofGetSystemTime() - time_one_frame;
		num_frames ++;
		if(ofGetSystemTime() - time_prev_out > 5000){
			time_prev_out = ofGetSystemTime();
			ofLog(OF_LOG_NOTICE,"avg time: %f" , float(acc_time)/float(num_frames));
		}*/
		env->ReleasePrimitiveArrayCritical(array,out_buffer,0);
		return 0;
	}
	return 1;
}

static float conv_factor = 1/32767.5f;

jint
Java_cc_openframeworks_OFAndroidSoundStream_audioReceived(JNIEnv*  env, jobject  thiz, jshortArray array, jint numChannels, jint bufferSize){
	if(!in_float_buffer || numChannels!=inChannels || bufferSize!=inBufferSize){
		if(in_float_buffer) delete[] in_float_buffer;
		in_float_buffer = new float[bufferSize*numChannels];
		inBufferSize = bufferSize;
		inChannels   = numChannels;
	}
	if(OFApp){
		in_buffer = (short*)env->GetPrimitiveArrayCritical(array, NULL);
		if(!in_buffer) return 1;
		for(int i=0;i<bufferSize*numChannels;i++){
			in_float_buffer[i] = (float(in_buffer[i]) + 0.5) * conv_factor;
		}
		OFApp->audioReceived(in_float_buffer,bufferSize,numChannels);
		env->ReleasePrimitiveArrayCritical(array,in_buffer,0);
		return 0;
	}
	return 1;
}
}
