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
	CLLocationManager *locationManager;
	
	//location
	double lat;
	double lng;
	double hAccuracy;
	double alt;
	double vAccuracy;
	double distMoved;
	//NSString * lastUpdatedLocationTimestamp
	
	//heading
	double x;
	double y;
	double z;
	double magneticHeading;
	double trueHeading;
	double headingAccuracy;
	//NSString * lastUpdatedCompasTimestamp
}

@property (nonatomic, readonly) double lat;
@property (nonatomic, readonly) double lng;
@property (nonatomic, readonly) double hAccuracy;
@property (nonatomic, readonly) double alt;
@property (nonatomic, readonly) double vAccuracy;
@property (nonatomic, readonly) double distMoved;
@property (nonatomic, readonly) double x;
@property (nonatomic, readonly) double y;
@property (nonatomic, readonly) double z;
@property (nonatomic, readonly) double magneticHeading;
@property (nonatomic, readonly) double trueHeading;
@property (nonatomic, readonly) double headingAccuracy;

- (instancetype) init;
- (void) dealloc;

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
		~ofxiOSCoreLocation();	
		
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

