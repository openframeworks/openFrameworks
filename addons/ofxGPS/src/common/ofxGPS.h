//
//  ofxGPS.h
//
//  Created by Tal Lavi on 10/2/14.
//
//

#pragma once

#include "ofMain.h"
#include "ILocationProvider.h"

struct ofxGPSData
{
    bool hasLocation;
    double longitude;
    double latitude;
    double locationAccuracy;
    
    bool hasAltitude;
    double altitude;
    double altitudeAccuracy;
    
    bool hasHeading;
    double heading;
    double headingAccuracy;
    
    Poco::Timestamp time;
};

class ofxGPS
{
    
public:
    
    static std::shared_ptr<ofxGPS> create();
    
    virtual ~ofxGPS() {}
    
    virtual ofxGPSData getGPSData() = 0;
    
    static ofEvent<const ofxGPSData> gpsDataChangedEvent;
};


