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

//void ofRunApp( ofxAndroidApp * app);


void ofxRegisterMultitouch(ofxAndroidApp * app);

void ofxAndroidAlertBox(string msg);
int ofxAndroidProgressBox(string msg);
void ofxAndroidDismissProgressBox(int id);
bool ofxAndroidOkCancelBox(string msg);

void ofxAndroidToast(string msg);

void ofxAndroidLockScreenSleep();
void ofxAndroidUnlockScreenSleep();

bool ofxAndroidIsOnline();
bool ofxAndroidIsWifiOnline();
bool ofxAndroidIsMobileOnline();

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


//this is just to fix a problem with undefined symbols
inline void ofFixSoundStreamInclude(){
	ofSoundStreamClose();
}
#endif /* OFANDROIDUTILS_H_ */
