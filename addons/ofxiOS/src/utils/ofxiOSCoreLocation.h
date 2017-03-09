/*
 *  ofIphoneCoreLocation.h
 *
 *  Created by Zach Gage on 3/1/09.
 *  Copyright 2009 stfj. All rights reserved.
 *
 */

#pragma once

#include <TargetConditionals.h>
#if TARGET_OS_IOS || (TARGET_OS_IPHONE && !TARGET_OS_TV)

#import <CoreLocation/CoreLocation.h>

@interface ofxiOSCoreLocationDelegate : NSObject <CLLocationManagerDelegate>
{
	
}

@property (nonatomic, assign) double lat;
@property (nonatomic, assign) double lng;
@property (nonatomic, assign) double hAccuracy;
@property (nonatomic, assign) double alt;
@property (nonatomic, assign) double vAccuracy;
@property (nonatomic, assign) double distMoved;
@property (nonatomic, assign) double x;
@property (nonatomic, assign) double y;
@property (nonatomic, assign) double z;
@property (nonatomic, assign) double magneticHeading;
@property (nonatomic, assign) double trueHeading;
@property (nonatomic, assign) double headingAccuracy;
@property (nonatomic, strong) CLLocationManager* locationManager;

- (id) init;

- (bool) startHeading;
- (void) stopHeading;

- (bool) startLocation;
- (void) stopLocation;

- (bool) startMonitoringSignificantLocationChanges;
- (void) stopMonitoringSignificantLocationChanges;

- (void)locationManager:(CLLocationManager *)manager
	didUpdateToLocation:(CLLocation *)newLocation
		   fromLocation:(CLLocation *)oldLocation;

- (void)locationManager:(CLLocationManager *)manager
	   didFailWithError:(NSError *)error;

@end

#define ofxiPhoneCoreLocationDelegate ofxiOSCoreLocationDelegate


class ofxiOSCoreLocation
{
		
	public:
		
		ofxiOSCoreLocation();
		
		bool startHeading();
		void stopHeading();
		
		bool startLocation();
		void stopLocation();
  
        bool startMonitoringSignificantLocationChanges();
        void stopMonitoringSignificantLocationChanges();	
  
		double getLatitude();
		double getLongitude();
		double getLocationAccuracy();
		double getAltitude();
		double getAltitudeAccuracy();
		double getDistMoved();
		double getCompassX();
		double getCompassY();
		double getCompassZ();
		double getMagneticHeading();
		double getTrueHeading();
		double getHeadingAccuracy();
		
	protected:
	
		ofxiOSCoreLocationDelegate *	coreLoc;
};

#define ofxiPhoneCoreLocation ofxiOSCoreLocation

#endif

