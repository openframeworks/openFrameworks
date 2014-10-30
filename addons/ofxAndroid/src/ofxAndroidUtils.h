/*
 * ofAndroidUtils.h
 *
 *  Created on: 10/07/2010
 *      Author: arturo
 */

#pragma once

#include <jni.h>
#include "ofxAndroidApp.h"
#include "ofSoundStream.h"

JavaVM * ofGetJavaVMPtr();
JNIEnv * ofGetJNIEnv();
jclass ofGetJavaOFAndroid();
jobject ofGetOFActivityObject();

void ofxAndroidAlertBox(string msg);
int ofxAndroidProgressBox(string msg);
void ofxAndroidDismissProgressBox(int id);
void ofxAndroidOkCancelBox(string msg);
void ofxAndroidYesNoBox(string msg);
void ofxAndroidAlertTextBox(string question, string text);
string ofxAndroidGetTextBoxResult();
bool ofxAndroidAlertListBox(string title, const vector<string> & list);

void ofxAndroidToast(string msg);

void ofxAndroidLockScreenSleep();
void ofxAndroidUnlockScreenSleep();

bool ofxAndroidIsOnline();
bool ofxAndroidIsWifiOnline();
bool ofxAndroidIsMobileOnline();

string ofxAndroidGetStringRes(string id);

string ofxAndroidRandomUUID();

void ofxAndroidMonitorNetworkState();

void ofxAndroidLaunchBrowser(string url);

void ofxAndroidNotifyLoadPercent(float percent);

bool ofxAndroidCheckSDCardMounted();

void ofxAndroidEnableMulticast();
void ofxAndroidDisableMulticast();

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

class ofxAndroidEventsClass{
public:
	ofEvent<bool> okPressed;
	ofEvent<bool> cancelPressed;
	ofEvent<bool> networkConnected;
};

ofxAndroidEventsClass & ofxAndroidEvents();
