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
	// android reports these in m/s^2, but ofxAccelerometer expects g's (1g = gravity = 9.81m/s^2)
	ofxAccelerometer.update(-x/9.81,-y/9.81,-z/9.81);
}
}

void ofxAccelerometerHandler::setup(){
	if(!ofGetJavaVMPtr()){
		ofLogError("ofxAndroidAccelerometer") << "setup(): couldn't find java virtual machine";
		return;
	}
	JNIEnv *env;
	if (ofGetJavaVMPtr()->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		ofLogError("ofxAndroidAccelerometer") << "setup(): failed to get environment using GetEnv()";
		return;
	}
	jclass javaClass = env->FindClass("cc/openframeworks/OFAndroid");

	if(javaClass==0){
		ofLogError("ofxAndroidAccelerometer") << "setup(): couldn't find OFAndroid java class";
		return;
	}

	jmethodID setupAccelerometer = env->GetStaticMethodID(javaClass,"setupAccelerometer","()V");
	if(!setupAccelerometer){
		ofLogError("ofxAndroidAccelerometer") << "setup(): couldn't find OFAndroid.setupAccelerometer method";
		return;
	}
	env->CallStaticVoidMethod(javaClass,setupAccelerometer);
}

void ofxAccelerometerHandler::exit(){

}
