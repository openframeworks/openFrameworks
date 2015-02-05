//
//  ofxGPS.h
//
//  Created by Tal Lavi on 10/2/14.
//
//

#pragma once

#include "ofMain.h"
#include "ILocationProvider.h"

class ofxGPS
{
    
private:
    
    static bool m_locationStarted;
    static bool m_headingStarted;
    
public:
    
    struct Data
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
    
    static bool locationStarted(){return m_locationStarted;}
    static bool headingStarted(){return m_headingStarted;}
    
    static bool startLocation();
    static bool startHeading();
    
    static void stopLocation();
    static void stopHeading();
    
    static ofxGPS::Data getGPSData();
    
    static ofEvent<const ofxGPS::Data> gpsDataChangedEvent;
};


