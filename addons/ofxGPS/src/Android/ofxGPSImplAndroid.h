//
//  ofxGPSImplAndroid.h
//
//  Created by Tal Lavi on 10/12/14.
//
//

#pragma once

#include "ofMain.h"
#include "ofxGPS.h"
#include "ofEvents.h"
#include "Poco/Mutex.h"
#include <jni.h>

struct ofxCompassData
{
	float heading;
};

class ofxGPSImplAndroid : public ofxGPS
{

private:

	jobject m_OFAndroidGPS;

	ofxGPSData m_gpsData;

	Poco::Mutex m_mutex;

public:

    ofxGPSImplAndroid();
    virtual ~ofxGPSImplAndroid();
    
    void startGPS();
    void stopGPS();
    void startCompass();
    void stopCompass();

    virtual ofxGPSData getGPSData();

private:
    void onNewGPSData(ofxGPSData& gpsData);
    void onNewCompassData(ofxCompassData& compassData);

public:

    static std::shared_ptr<ofxGPS> create();

    static ofEvent<ofxGPSData> newGPSDataEvent;
    static ofEvent<ofxCompassData> newCompassDataEvent;
};


