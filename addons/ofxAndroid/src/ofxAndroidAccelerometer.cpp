/*
 * ofxAndroidAccelerometer.cpp
 *
 *  Created on: 28/11/2010
 *      Author: arturo
 */

#include <jni.h>
#include "ofxAccelerometer.h"
#include "ofxAndroidUtils.h"
#include "ofLog.h"

extern "C"{
void
Java_cc_openframeworks_OFAndroidAccelerometer_updateAccelerometer( JNIEnv*  env, jobject  thiz, jfloat x, jfloat y, jfloat z ){
	ofxAccelerometer.update(-x,-y,-z);
}
}

void ofxAccelerometerHandler::setup(){
	if(!ofGetJavaVMPtr()){
		ofLog(OF_LOG_ERROR,"ofxAccelerometerHandler: Cannot find java virtual machine");
		return;
	}
	JNIEnv *env;
	if (ofGetJavaVMPtr()->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return;
	}
	jclass javaClass = env->FindClass("cc/openframeworks/OFAndroid");

	if(javaClass==0){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid java class");
		return;
	}

	jmethodID setupAccelerometer = env->GetStaticMethodID(javaClass,"setupAccelerometer","()V");
	if(!setupAccelerometer){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid.setupAccelerometer method");
		return;
	}
	env->CallStaticVoidMethod(javaClass,setupAccelerometer);
}

void ofxAccelerometerHandler::exit(){

}
