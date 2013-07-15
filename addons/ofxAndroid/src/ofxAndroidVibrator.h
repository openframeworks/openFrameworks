/*
 * ofxAndroidVibrator.h
 *
 *  Created on: Feb 21, 2013
 *      Author: arturo
 */

#ifndef OFXANDROIDVIBRATOR_H_
#define OFXANDROIDVIBRATOR_H_
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
	static jmethodID getVibratorMethodID(string name, string signature);
};

#endif /* OFXANDROIDVIBRATOR_H_ */
