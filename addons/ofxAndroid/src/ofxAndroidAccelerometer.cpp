/*
 * ofxAndroidAccelerometer.cpp
 *
 *  Created on: 28/11/2010
 *      Author: arturo
 */

#include <jni.h>
#include "ofxAccelerometer.h"

extern "C"{
void
Java_cc_openframeworks_OFAndroidAccelerometer_updateAccelerometer( JNIEnv*  env, jobject  thiz, jfloat x, jfloat y, jfloat z ){
	ofxAccelerometer.update(x,y,z);
}
}

void ofxAccelerometerHandler::setup(){

}

void ofxAccelerometerHandler::exit(){

}
