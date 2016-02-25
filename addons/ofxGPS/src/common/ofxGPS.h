//
//  ofxGPS.h
//
//  Created by Tal Lavi on 10/2/14.
//
//

#pragma once

#include "ofMain.h"

class ofxGPS
{
    
private:
    
    static bool m_locationStarted;
    static bool m_headingStarted;
    
public:
    
    struct LocationData
    {
        bool hasLocation;
        double longitude;
        double latitude;
        double locationAccuracy;
        
        bool hasAltitude;
        double altitude;
        double altitudeAccuracy;
    };
    
    struct HeadingData
    {
        bool hasHeading;
        double heading;
        double headingAccuracy;
    };
    
    static bool locationStarted(){return m_locationStarted;}
    static bool headingStarted(){return m_headingStarted;}
    
    static bool startLocation();
    static bool startHeading();
    
    static void stopLocation();
    static void stopHeading();
    
    static ofEvent<const ofxGPS::LocationData> newLocationDataEvent;
    static ofEvent<const ofxGPS::HeadingData> newHeadingDataEvent;
};


