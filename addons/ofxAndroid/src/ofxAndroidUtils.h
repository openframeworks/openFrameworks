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

jmethodID ofxJavaGetMethodID(jclass classID, std::string methodName, std::string methodSignature);
jmethodID ofxJavaGetStaticMethodID(jclass classID, std::string methodName, std::string methodSignature);
std::string ofxJavaGetClassName(jclass classID);
jclass ofxJavaGetClassID(std::string className);
jfieldID ofxJavaGetStaticFieldID(jclass classID, std::string fieldName, std::string fieldType);

jobject ofxJavaGetStaticObjectField(jclass classID, std::string fieldName, std::string fieldType);
jobject ofxJavaGetStaticObjectField(std::string className, std::string fieldName, std::string fieldType);

void ofxJavaCallVoidMethod(jobject object, jclass classID, std::string methodName, std::string methodSignature, va_list args);
void ofxJavaCallVoidMethod(jobject object, jclass classID, std::string methodName, std::string methodSignature, ...);
void ofxJavaCallVoidMethod(jobject object, std::string className, std::string methodName, std::string methodSignature, ...);

jobject ofxJavaCallStaticObjectMethod(jclass classID, std::string methodName, std::string methodSignature, va_list args);
jobject ofxJavaCallStaticObjectMethod(jclass classID, std::string methodName, std::string methodSignature, ...);
jobject ofxJavaCallStaticObjectMethod(std::string className, std::string methodName, std::string methodSignature, ...);

jobject ofxJavaCallObjectMethod(jobject object, jclass classID, std::string methodName, std::string methodSignature, va_list args);
jobject ofxJavaCallObjectMethod(jobject object, jclass classID, std::string methodName, std::string methodSignature, ...);
jobject ofxJavaCallObjectMethod(jobject object, std::string className, std::string methodName, std::string methodSignature, ...);

void ofxJavaCallStaticVoidMethod(jclass classID, std::string methodName, std::string methodSignature, va_list args);
void ofxJavaCallStaticVoidMethod(jclass classID, std::string methodName, std::string methodSignature, ...);
void ofxJavaCallStaticVoidMethod(std::string className, std::string methodName, std::string methodSignature, ...);

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
