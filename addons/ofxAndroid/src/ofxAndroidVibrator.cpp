/*
 * ofxAndroidVibrator.cpp
 *
 *  Created on: Feb 21, 2013
 *      Author: arturo
 */

#include "ofxAndroidVibrator.h"
#include "ofxAndroidUtils.h"
#include "ofLog.h"
#include <jni.h>

using namespace std;

ofxAndroidVibrator::ofxAndroidVibrator() {
	// TODO Auto-generated constructor stub

}

ofxAndroidVibrator::~ofxAndroidVibrator() {
	// TODO Auto-generated destructor stub
}

jobject ofxAndroidVibrator::getVibratorService(){
	jobject activity = ofGetOFActivityObject();
	jclass contextClass = ofGetJNIEnv()->FindClass("android/content/Context");
	if(!contextClass){
		ofLogError("ofxAndroidVibrator") << "getVibratorService(): couldn't get Context class";
		return 0;
	}
	jmethodID method = ofGetJNIEnv()->GetMethodID(contextClass,"getSystemService","(Ljava/lang/String;)Ljava/lang/Object;");
	if(!method){
		ofLogError("ofxAndroidVibrator") << "getVibratorService(): getSystemService not found";
		return 0;
	}
	jfieldID vibratorServiceField = ofGetJNIEnv()->GetStaticFieldID(contextClass,"VIBRATOR_SERVICE","Ljava/lang/String;");
	if(!vibratorServiceField){
		ofLogError("ofxAndroidVibrator") << "getVibratorService(): VIBRATOR_SERVICE id not found";
		return 0;
	}
	jstring vibratorServiceStr = (jstring)ofGetJNIEnv()->GetStaticObjectField(contextClass,vibratorServiceField);
	if(!vibratorServiceStr){
		ofLogError("ofxAndroidVibrator") << "getVibratorService(): couldn't get VIBRATOR_SERVICE value";
		return 0;
	}
	jobject vibratorService = ofGetJNIEnv()->CallObjectMethod(activity,method,vibratorServiceStr);
	if(!vibratorService){
		ofLogError("ofxAndroidVibrator") << "getVibratorService(): couldn't call getSystemService";
		return 0;
	}

	return vibratorService;
}

jmethodID ofxAndroidVibrator::getVibratorMethodID(string name, string signature){
	jclass vibratorClass = ofGetJNIEnv()->FindClass("android/os/Vibrator");
	if(!vibratorClass){
		ofLogError("ofxAndroidVibrator") << "getVibratorMethodID(): couldn't get Vibrator class";
		return 0;
	}
	return ofGetJNIEnv()->GetMethodID(vibratorClass,name.c_str(),signature.c_str());
}

void ofxAndroidVibrator::vibrate(unsigned long milliseconds){
	jobject vibratorService = getVibratorService();

	jmethodID vibrateMethod = getVibratorMethodID("vibrate","(J)V");
	if(!vibrateMethod){
		ofLogError("ofxAndroidVibrator") << "vibrate(): couldn't get vibrate method";
		return;
	}
	ofGetJNIEnv()->CallVoidMethod(vibratorService,vibrateMethod,(jlong)milliseconds);
}

void ofxAndroidVibrator::stop(){
	jobject vibratorService = getVibratorService();

	jmethodID vibrateMethod = getVibratorMethodID("cancel","()V");
	if(!vibrateMethod){
		ofLogError("ofxAndroidVibrator") << "stop(): couldn't get cancel method";
		return;
	}
	ofGetJNIEnv()->CallVoidMethod(vibratorService,vibrateMethod);
}
