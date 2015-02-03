//
//  ofxGPSImplAndroid.cpp
//
//  Created by Tal Lavi on 10/12/14.
//
//

#include "ofxGPSImplAndroid.h"
#include "ofMain.h"
#include "ofxAndroidUtils.h"

ofEvent<ofxGPSData> ofxGPSImplAndroid::newGPSDataEvent;
ofEvent<ofxCompassData> ofxGPSImplAndroid::newCompassDataEvent;

ofEvent<const ofxGPSData> ofxGPS::gpsDataChangedEvent;

ofxGPSImplAndroid::ofxGPSImplAndroid()
{
	ofAddListener(ofxGPSImplAndroid::newGPSDataEvent, this, &ofxGPSImplAndroid::onNewGPSData);
	ofAddListener(ofxGPSImplAndroid::newCompassDataEvent, this, &ofxGPSImplAndroid::onNewCompassData);

	m_OFAndroidGPS = ofxJavaCallStaticObjectMethod("cc/openframeworks/OFAndroidGPS", "getInstance", "()Lcc/openframeworks/OFAndroidGPS;");

    startGPS();
    startCompass();
}

ofxGPSImplAndroid::~ofxGPSImplAndroid()
{
	stopGPS();

	ofRemoveListener(ofxGPSImplAndroid::newGPSDataEvent, this, &ofxGPSImplAndroid::onNewGPSData);
	ofRemoveListener(ofxGPSImplAndroid::newCompassDataEvent, this, &ofxGPSImplAndroid::onNewCompassData);
}

ofxGPSData ofxGPSImplAndroid::getGPSData()
{
	Poco::Mutex::ScopedLock lock(m_mutex);

	return m_gpsData;
}

void ofxGPSImplAndroid::onNewGPSData(ofxGPSData& gpsData)
{
	Poco::Mutex::ScopedLock lock(m_mutex);

	m_gpsData.hasLocation = gpsData.hasLocation;
	m_gpsData.longitude = gpsData.longitude;
	m_gpsData.latitude = gpsData.latitude;
	m_gpsData.locationAccuracy = gpsData.locationAccuracy;
	m_gpsData.hasAltitude = gpsData.hasAltitude;
	m_gpsData.altitude = gpsData.altitude;
	m_gpsData.altitudeAccuracy = gpsData.altitudeAccuracy;
	m_gpsData.time = gpsData.time;

	ofNotifyEvent(ofxGPS::gpsDataChangedEvent, m_gpsData);
}

void ofxGPSImplAndroid::onNewCompassData(ofxCompassData& compassData)
{
	Poco::Mutex::ScopedLock lock(m_mutex);

	m_gpsData.hasHeading = true;
	m_gpsData.heading = compassData.heading;

	ofNotifyEvent(ofxGPS::gpsDataChangedEvent, m_gpsData);
}

std::shared_ptr<ofxGPS> ofxGPS::create()
{
    return std::shared_ptr<ofxGPS>(new ofxGPSImplAndroid());
}

void ofxGPSImplAndroid::startGPS(){

	ofxJavaCallVoidMethod(m_OFAndroidGPS, "cc/openframeworks/OFAndroidGPS", "startGPS",  "()V");
}

void ofxGPSImplAndroid::stopGPS(){

	ofxJavaCallVoidMethod(m_OFAndroidGPS, "cc/openframeworks/OFAndroidGPS", "stopGPS",  "()V");
}

void ofxGPSImplAndroid::startCompass(){

	ofxJavaCallVoidMethod(m_OFAndroidGPS, "cc/openframeworks/OFAndroidGPS", "startCompass",  "()V");
}

void ofxGPSImplAndroid::stopCompass(){

	ofxJavaCallVoidMethod(m_OFAndroidGPS, "cc/openframeworks/OFAndroidGPS", "stopCompass",  "()V");
}

extern "C"{
void
Java_cc_openframeworks_OFAndroidGPS_locationChanged( JNIEnv*  env, jobject  thiz, jdouble altitude, jdouble latitude, jdouble longitude, jfloat speed, jfloat heading ){

	ofxGPSData gpsData;

	gpsData.time = Poco::Timestamp();

	gpsData.hasLocation = true;
	gpsData.longitude = longitude;
	gpsData.latitude = latitude;

	gpsData.hasAltitude = true;
	gpsData.altitude = altitude;

	//gpsData.hasHeading = true;
	//gpsData.heading = heading;

	ofNotifyEvent(ofxGPSImplAndroid::newGPSDataEvent, gpsData);
}

void
Java_cc_openframeworks_OFAndroidGPS_headingChanged( JNIEnv*  env, jobject  thiz, jdouble heading ){

	ofxCompassData compassData;

	compassData.heading = heading;

	ofNotifyEvent(ofxGPSImplAndroid::newCompassDataEvent, compassData);
}

}

