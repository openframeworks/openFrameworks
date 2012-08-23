#include "ofxAndroidGPS.h"
#include "ofxAndroidUtils.h"
#include <jni.h>

ofEvent<ofxLocation> ofxAndroidGPS::locationChangedE;


void ofxAndroidGPS::startGPS(){
	jclass OFAndroid = ofGetJavaOFAndroid();

	if(OFAndroid==0){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid java class");
		return;
	}

	jmethodID setupGPS = ofGetJNIEnv()->GetStaticMethodID(OFAndroid,"setupGPS","()V");
	if(!setupGPS){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid.setupGPS method");
		return;
	}
	ofGetJNIEnv()->CallStaticVoidMethod(OFAndroid,setupGPS);
}

void ofxAndroidGPS::stopGPS(){
	jclass OFAndroid = ofGetJavaOFAndroid();

	if(OFAndroid==0){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid java class");
		return;
	}

	jmethodID stopGPS = ofGetJNIEnv()->GetStaticMethodID(OFAndroid,"stopGPS","()V");
	if(!stopGPS){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid.stopGPS method");
		return;
	}
	ofGetJNIEnv()->CallStaticVoidMethod(OFAndroid,stopGPS);
}

extern "C"{
void
Java_cc_openframeworks_OFAndroidGPS_locationChanged( JNIEnv*  env, jobject  thiz, jdouble altitude, jdouble latitude, jdouble longitude, jfloat speed, jfloat bearing ){
	ofxLocation location = {altitude, latitude, longitude, speed, bearing};
	ofNotifyEvent(ofxAndroidGPS::locationChangedE,location);
}
}
