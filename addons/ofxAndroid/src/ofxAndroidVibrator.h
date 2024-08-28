/*
 * ofxAndroidVibrator.h
 *
 *  Created on: Feb 21, 2013
 *      Author: arturo
 */

#pragma once

#include <jni.h>
#include "ofConstants.h"

class ofxAndroidVibrator {
public:
	ofxAndroidVibrator();
	virtual ~ofxAndroidVibrator();

	static void vibrate(unsigned long milliseconds);
	static void stop();
private:
	static jobject getVibratorService();
	static jmethodID getVibratorMethodID(std::string name, std::string signature);
};
