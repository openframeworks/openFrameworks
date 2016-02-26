//
//  ofxGPSImplAndroid.cpp
//
//  Created by Tal Lavi on 10/12/14.
//
//

#include "ofMain.h"
#include "ofxAndroidUtils.h"
#include "ofxGPS.h"

bool ofxGPS::m_locationStarted;
bool ofxGPS::m_headingStarted;

ofEvent<const ofxGPS::LocationData> ofxGPS::newLocationDataEvent;
ofEvent<const ofxGPS::HeadingData> ofxGPS::newHeadingDataEvent;

bool isInitialized = false;

jobject OFAndroidGPS;

jobject& getJavaInstanceSafe()
{
	if (!isInitialized)
		OFAndroidGPS = ofxJavaCallStaticObjectMethod("cc/openframeworks/OFAndroidGPS", "getInstance", "()Lcc/openframeworks/OFAndroidGPS;");

	return OFAndroidGPS;
}

bool ofxGPS::startLocation(){

	ofxJavaCallVoidMethod(getJavaInstanceSafe(), "cc/openframeworks/OFAndroidGPS", "startGPS",  "()V");

	m_locationStarted = true;

	return true;
}

void ofxGPS::stopLocation(){

	ofxJavaCallVoidMethod(getJavaInstanceSafe(), "cc/openframeworks/OFAndroidGPS", "stopGPS",  "()V");

	m_locationStarted = false;
}

bool ofxGPS::startHeading(){

	ofxJavaCallVoidMethod(getJavaInstanceSafe(), "cc/openframeworks/OFAndroidGPS", "startCompass",  "()V");

	m_headingStarted = true;

	return true;
}

void ofxGPS::stopHeading(){

	ofxJavaCallVoidMethod(getJavaInstanceSafe(), "cc/openframeworks/OFAndroidGPS", "stopCompass",  "()V");

	m_headingStarted = false;
}

extern "C"{
void
Java_cc_openframeworks_OFAndroidGPS_locationChanged( JNIEnv*  env, jobject  thiz, jdouble altitude, jdouble latitude, jdouble longitude, jfloat speed, jfloat heading ){

	ofxGPS::LocationData locationData;

	locationData.hasLocation = true;
	locationData.longitude = longitude;
	locationData.latitude = latitude;

	locationData.hasAltitude = true;
	locationData.altitude = altitude;

	ofNotifyEvent(ofxGPS::newLocationDataEvent, locationData);
}

void
Java_cc_openframeworks_OFAndroidGPS_headingChanged( JNIEnv*  env, jobject  thiz, jdouble heading ){

	ofxGPS::HeadingData headingData;
	headingData.hasHeading = true;
	headingData.heading = heading;

	ofNotifyEvent(ofxGPS::newHeadingDataEvent, headingData);
}

}

