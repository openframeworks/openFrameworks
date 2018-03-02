/*
 *  ofxGPSImpliOS.h
 *
 *  Created by Tal Lavi on 10/2/14.
 *  Based on ofxiOSCoreLocation.h by Zach Gage
 *
 *
 */

#import <UIKit/UIKit.h>
#import <CoreLocation/CoreLocation.h>
#import "ofMain.h"
#import "ofxGPS.h"

#pragma once

@interface ofxGPSImpliOSCoreLocationDelegate : NSObject <CLLocationManagerDelegate>
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
    
    bool m_didReportLocation;
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

- (id) init;
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

#define ofxiPhoneCoreLocation ofxiOSCoreLocation

