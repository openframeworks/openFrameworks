/*
 * ofAndroidUtils.h
 *
 *  Created on: 10/07/2010
 *      Author: arturo
 */

#pragma once

#include <jni.h>
#include "ofConstants.h"
#include "ofEvent.h"

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

float ofxJavaCallFloatMethod(jobject object, jclass classID, std::string methodName, std::string methodSignature, va_list args);
float ofxJavaCallFloatMethod(jobject object, jclass classID, std::string methodName, std::string methodSignature, ...);
float ofxJavaCallFloatMethod(jobject object, std::string className, std::string methodName, std::string methodSignature, ...);

int ofxJavaCallIntMethod(jobject object, jclass classID, std::string methodName, std::string methodSignature, va_list args);
int ofxJavaCallIntMethod(jobject object, jclass classID, std::string methodName, std::string methodSignature, ...);
int ofxJavaCallIntMethod(jobject object, std::string className, std::string methodName, std::string methodSignature, ...);

int64_t ofxJavaCallLongMethod(jobject object, jclass classID, std::string methodName, std::string methodSignature, va_list args);
int64_t ofxJavaCallLongMethod(jobject object, jclass classID, std::string methodName, std::string methodSignature, ...);
int64_t ofxJavaCallLongMethod(jobject object, std::string className, std::string methodName, std::string methodSignature, ...);

bool ofxJavaCallBoolMethod(jobject object, jclass classID, std::string methodName, std::string methodSignature, va_list args);
bool ofxJavaCallBoolMethod(jobject object, jclass classID, std::string methodName, std::string methodSignature, ...);
bool ofxJavaCallBoolMethod(jobject object, std::string className, std::string methodName, std::string methodSignature, ...);

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

void ofxAndroidSetViewItemChecked(string item_name, bool checked);

enum ofxAndroidSwipeDir{
	OFX_ANDROID_SWIPE_UP    = 1,
	OFX_ANDROID_SWIPE_DOWN  = 2,
	OFX_ANDROID_SWIPE_LEFT  = 3,
	OFX_ANDROID_SWIPE_RIGHT = 4
};

struct ofxAndroidSwipeEventArgs{
	ofxAndroidSwipeDir dir;
	int id;
};

/// pinch to zoom style gestures
/// https://developer.android.com/reference/android/view/ScaleGestureDetector.html
class ofxAndroidScaleEventArgs{
public:
	ofxAndroidScaleEventArgs(jobject detector);

	/// Return the average distance between each of the pointers forming the gesture in progress through the focal point.
	float 	getCurrentSpan();
	/// Return the average X distance between each of the pointers forming the gesture in progress through the focal point.
	float 	getCurrentSpanX();
	/// Return the average Y distance between each of the pointers forming the gesture in progress through the focal point.
	float 	getCurrentSpanY();
	/// Return the event time of the current event being processed.
	int64_t 	getEventTime();
	/// Get the X coordinate of the current gesture's focal point.
	float 	getFocusX();
	/// Get the Y coordinate of the current gesture's focal point.
	float 	getFocusY();
	/// Return the previous average distance between each of the pointers forming the gesture in progress through the focal point.
	float 	getPreviousSpan();
	/// Return the previous average X distance between each of the pointers forming the gesture in progress through the focal point.
	float 	getPreviousSpanX();
	/// Return the previous average Y distance between each of the pointers forming the gesture in progress through the focal point.
	float 	getPreviousSpanY();
	/// Return the scaling factor from the previous scale event to the current event.
	float 	getScaleFactor();
	/// Return the time difference in milliseconds between the previous accepted scaling event and the current scaling event.
	int64_t 	getTimeDelta();

private:
	jobject detector;
};

class ofxAndroidEventsClass{
public:
	ofEvent<void> okPressed;
	ofEvent<void> cancelPressed;
	ofEvent<void> backPressed;
	ofEvent<bool> networkConnected;
	ofEvent<void> pause;
	ofEvent<void> resume;
	ofEvent<void> unloadGL;
	ofEvent<void> reloadGL;
	ofEvent<ofxAndroidSwipeEventArgs> swipe;
	ofEvent<ofxAndroidScaleEventArgs> scale;
	ofEvent<ofxAndroidScaleEventArgs> scaleBegin;
	ofEvent<ofxAndroidScaleEventArgs> scaleEnd;

	ofEvent<std::string> menuItemSelected;
	ofEvent<std::string> menuItemChecked;

};

ofxAndroidEventsClass & ofxAndroidEvents();
