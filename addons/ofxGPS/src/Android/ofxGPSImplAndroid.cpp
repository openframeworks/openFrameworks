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

ofEvent<const ofxGPS::Data> ofxGPS::gpsDataChangedEvent;

bool isInitialized = false;

jobject OFAndroidGPS;

ofxGPS::Data gpsData;

Poco::Mutex mutex;

jobject& getJavaInstanceSafe()
{
	if (!isInitialized)
		OFAndroidGPS = ofxJavaCallStaticObjectMethod("cc/openframeworks/OFAndroidGPS", "getInstance", "()Lcc/openframeworks/OFAndroidGPS;");

	return OFAndroidGPS;
}

ofxGPS::Data ofxGPS::getGPSData()
{
	Poco::Mutex::ScopedLock lock(mutex);

	return gpsData;
}

void updateGPSData(ofxGPS::Data& newGpsData)
{

	Poco::Mutex::ScopedLock lock(mutex);

	gpsData.hasLocation = newGpsData.hasLocation;
	gpsData.longitude = newGpsData.longitude;
	gpsData.latitude = newGpsData.latitude;
	gpsData.locationAccuracy = newGpsData.locationAccuracy;
	gpsData.hasAltitude = newGpsData.hasAltitude;
	gpsData.altitude = newGpsData.altitude;
	gpsData.altitudeAccuracy = newGpsData.altitudeAccuracy;
	gpsData.time = newGpsData.time;

	ofNotifyEvent(ofxGPS::gpsDataChangedEvent, gpsData);
}

void updateCompassData(double heading)
{

	Poco::Mutex::ScopedLock lock(mutex);

	gpsData.hasHeading = true;
	gpsData.heading = heading;

	ofNotifyEvent(ofxGPS::gpsDataChangedEvent, gpsData);
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

	ofxGPS::Data gpsData;

	gpsData.time = Poco::Timestamp();

	gpsData.hasLocation = true;
	gpsData.longitude = longitude;
	gpsData.latitude = latitude;

	gpsData.hasAltitude = true;
	gpsData.altitude = altitude;

	updateGPSData(gpsData);
}

void
Java_cc_openframeworks_OFAndroidGPS_headingChanged( JNIEnv*  env, jobject  thiz, jdouble heading ){

	updateCompassData(heading);
}

}

