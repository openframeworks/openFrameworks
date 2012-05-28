/*
 * ofAndroidUtils.h
 *
 *  Created on: 10/07/2010
 *      Author: arturo
 */

#ifndef OFANDROIDUTILS_H_
#define OFANDROIDUTILS_H_

#include <jni.h>
#include "ofxAndroidApp.h"
#include "ofSoundStream.h"

JavaVM * ofGetJavaVMPtr();
JNIEnv * ofGetJNIEnv();
jclass ofGetJavaOFAndroid();
jobject ofGetOFActivityObject();

//void ofRunApp( ofxAndroidApp * app);


void ofxRegisterMultitouch(ofxAndroidApp * app);

void ofxAndroidAlertBox(string msg);
int ofxAndroidProgressBox(string msg);
void ofxAndroidDismissProgressBox(int id);
void ofxAndroidOkCancelBox(string msg);

void ofxAndroidToast(string msg);

void ofxAndroidLockScreenSleep();
void ofxAndroidUnlockScreenSleep();

bool ofxAndroidIsOnline();
bool ofxAndroidIsWifiOnline();
bool ofxAndroidIsMobileOnline();

string ofxAndroidGetStringRes(string id);

string ofxAndroidRandomUUID();

//-------------------------------------
// this functions are only for internal use
void ofPauseVideoGrabbers();
void ofResumeVideoGrabbers();

void ofReloadAllImageTextures();

void ofUnloadAllFontTextures();
void ofReloadAllFontTextures();

void  ofUpdateBitmapCharacterTexture();

void ofxAndroidSoundStreamPause();
void ofxAndroidSoundStreamResume();

bool ofxAndroidCheckSDCardMounted();

//this is just to fix a problem with undefined symbols
inline void ofFixSoundStreamInclude(){
	ofSoundStreamClose();
}

inline void ofxAndroidSetViewItemChecked(string item_name, bool checked){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLog(OF_LOG_ERROR,"ofxAndroidSetViewItemChecked: cannot find OFAndroid java class");
		return;
	}

	jmethodID setViewItemChecked = ofGetJNIEnv()->GetStaticMethodID(javaClass,"setViewItemChecked","(Ljava/lang/String;Z)V");
	if(!setViewItemChecked){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid setViewItemChecked method");
		return;
	}
	ofGetJNIEnv()->CallStaticVoidMethod(javaClass,setViewItemChecked,ofGetJNIEnv()->NewStringUTF(item_name.c_str()),checked);
}
#endif /* OFANDROIDUTILS_H_ */
