/*
 * ofxAndroidMagneticField.p
 *
 *  Created on: 15/05/2013
 *      Author: p
 */

#include "ofxAndroidMagneticField.h"
#include "ofxAndroidUtils.h"
#include "ofLog.h"
#include <set>

//---------------------------------------------------------------------------
ofxAndroidMagneticField::ofxAndroidMagneticField() {

	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"ofxAndroidMagneticField :: Failed to get the environment using GetEnv()");
		return;
	}

	jclass localClass = env->FindClass("cc/openframeworks/OFAndroid");
	javaClass = (jclass) env->NewGlobalRef(localClass);

	if(!javaClass){
		ofLog(OF_LOG_ERROR,"Failed to get OFAndroid java class");
		return;
	}

};

//---------------------------------------------------------------------------
ofxAndroidMagneticField::~ofxAndroidMagneticField() {

	JNIEnv *env = ofGetJNIEnv();
	if(javaMagneticField) env->DeleteGlobalRef(javaMagneticField);
	if(javaMagneticFieldClass) env->DeleteGlobalRef(javaMagneticFieldClass);
	if(javaClass) env->DeleteGlobalRef(javaClass);

};

//---------------------------------------------------------------------------
bool ofxAndroidMagneticField::startHeading() {

	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return 0;
	}

	jmethodID constructor = env->GetStaticMethodID(javaClass,"setupMagneticField","()Lcc/openframeworks/OFAndroidMagneticField;");
	if(!constructor){
		ofLog(OF_LOG_ERROR,"Failed to get the java reference for MageticField");
		return 0;
	}
	javaMagneticField = env->CallObjectMethod(javaClass,constructor);
	if(!javaMagneticField){
		ofLog(OF_LOG_ERROR,"Failed to create java MageticField");
		return 0;
	}
	javaMagneticField = (jobject)env->NewGlobalRef(javaMagneticField);

	jclass localClass = env->FindClass("cc/openframeworks/OFAndroidMagneticField");
	javaMagneticFieldClass = (jclass) env->NewGlobalRef(localClass);

	jmethodID javaHasCompassMethod = env->GetMethodID(javaMagneticFieldClass,"hasCompass","()Z");
	if(!javaHasCompassMethod){
			ofLog(OF_LOG_ERROR,"Failed to get the java hasCompass for MagneticField");
			return 0;
	}

	return env->CallBooleanMethod(javaMagneticField,javaHasCompassMethod);

}

//---------------------------------------------------------------------------
float ofxAndroidMagneticField::getMagneticHeading() {

	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return 0;
	}

	jmethodID javaGetOrientationMethod = env->GetMethodID(javaMagneticFieldClass,"getOrientation","()F");
	if(!javaGetOrientationMethod){
			ofLog(OF_LOG_ERROR,"Failed to get the java getOrientation for MagneticField");
			return 0;
	}

	return env->CallFloatMethod(javaMagneticField,javaGetOrientationMethod);

}

//---------------------------------------------------------------------------
float ofxAndroidMagneticField::getTrueHeading() {

	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return 0;
	}

	jmethodID javaGetTrueOrientationMethod = env->GetMethodID(javaMagneticFieldClass,"getTrueOrientation","()F");
	if(!javaGetTrueOrientationMethod){
			ofLog(OF_LOG_ERROR,"Failed to get the java getTrueOrientation for MagneticField");
			return 0;
	}

	return env->CallFloatMethod(javaMagneticField,javaGetTrueOrientationMethod);

}
