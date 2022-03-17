/*
 *  ofxiOSCoreLocation.cpp
 *  iPhone CoreLocation Example
 *
 *  Created by Zach Gage on 3/1/09.
 *  Copyright 2009 stfj. All rights reserved.
 *
 *
 *
 * -------------------- note -----------------------
 *
 * To get the compass to work on 3GS+ phones you'll need to double clock on the iPhone oF target from the targets
 * dropdown in the grops & files menu. Make sure 'All Configurations' is selected, and then find the option called
 * 'Preprocessor Macros'. Add to that: OF_TARGET_BUILD_SDK_IS_3_PLUS
 * -------------------------------------------------
 *
 *
 */

#include "ofxiOSCoreLocation.h"

#include <TargetConditionals.h>
#if TARGET_OS_IOS || (TARGET_OS_IPHONE && !TARGET_OS_TV)

//C++ class implementations

//--------------------------------------------------------------
ofxiOSCoreLocation::ofxiOSCoreLocation()
{
	coreLoc = [[ofxiOSCoreLocationDelegate alloc] init];
}

//--------------------------------------------------------------
ofxiOSCoreLocation::~ofxiOSCoreLocation()
{
}

//--------------------------------------------------------------

bool ofxiOSCoreLocation::startHeading()
{
	return [coreLoc startHeading];
}

//--------------------------------------------------------------

void ofxiOSCoreLocation::stopHeading()
{
	[coreLoc stopHeading];
}

//--------------------------------------------------------------
bool ofxiOSCoreLocation::startLocation()
{
	return [coreLoc startLocation];
}

//--------------------------------------------------------------
void ofxiOSCoreLocation::stopLocation()
{
	[coreLoc stopLocation];
}

//--------------------------------------------------------------
bool ofxiOSCoreLocation::startMonitoringSignificantLocationChanges() {
  return [coreLoc startMonitoringSignificantLocationChanges];
}

//--------------------------------------------------------------
void ofxiOSCoreLocation::stopMonitoringSignificantLocationChanges() {
  [coreLoc stopMonitoringSignificantLocationChanges];
}

//--------------------------------------------------------------

double ofxiOSCoreLocation::getLatitude()
{
	return [coreLoc lat];
}

//--------------------------------------------------------------
double ofxiOSCoreLocation::getLongitude()
{
	return [coreLoc lng];
}

//--------------------------------------------------------------
double ofxiOSCoreLocation::getLocationAccuracy()
{
	return [coreLoc hAccuracy];
}

//--------------------------------------------------------------
double ofxiOSCoreLocation::getAltitude()
{
	return [coreLoc alt];
}

//--------------------------------------------------------------
double ofxiOSCoreLocation::getAltitudeAccuracy()
{
	return [coreLoc vAccuracy];
}

//--------------------------------------------------------------
double ofxiOSCoreLocation::getDistMoved()
{
	return [coreLoc distMoved];
}

//--------------------------------------------------------------
double ofxiOSCoreLocation::getCompassX()
{
	return [coreLoc x];
}

//--------------------------------------------------------------
double ofxiOSCoreLocation::getCompassY()
{
	return [coreLoc y];
}

//--------------------------------------------------------------
double ofxiOSCoreLocation::getCompassZ()
{
	return [coreLoc z];
}

//--------------------------------------------------------------
double ofxiOSCoreLocation::getMagneticHeading()
{
	return [coreLoc magneticHeading];
}

//--------------------------------------------------------------
double ofxiOSCoreLocation::getTrueHeading()
{
	return [coreLoc trueHeading];
}

//--------------------------------------------------------------
double ofxiOSCoreLocation::getHeadingAccuracy()
{
	return [coreLoc headingAccuracy];
}



//--------------------------------------------------------------

// CLASS IMPLEMENTATIONS--------------objc------------------------
//----------------------------------------------------------------
@implementation ofxiOSCoreLocationDelegate

//--------------------------------------------------------------
//create getter/setter functions for these variables
@synthesize lat, lng, hAccuracy, alt, vAccuracy, distMoved, x, y, z, magneticHeading, trueHeading, headingAccuracy;

//--------------------------------------------------------------
- (instancetype) init
{
	if(self = [super init])
	{		
		
		lat = 0;
		lng = 0;
		hAccuracy = 0;
		alt = 0;
		vAccuracy = 0;
		distMoved = 0;
		
		
		x = 0;
		y = 0;
		z = 0;
		magneticHeading = 0;
		trueHeading = 0;
		headingAccuracy = 0;
		
		
		locationManager = [[CLLocationManager alloc] init];
		locationManager.delegate = self;
	}
	return self;
}

//--------------------------------------------------------------
- (void)dealloc 
{ 
    locationManager = nil;
}

//--------------------------------------------------------------

- (bool) startHeading
{
	if([CLLocationManager headingAvailable])
	{
		[locationManager startUpdatingHeading];
		return true;
	}
	else
		return false;
}

//--------------------------------------------------------------

- (void) stopHeading
{
	[locationManager stopUpdatingHeading];
}

//--------------------------------------------------------------

- (bool) startLocation
{
	if([CLLocationManager locationServicesEnabled])
	{
		[locationManager startUpdatingLocation];
		return true;
	}
	else
		return false;
	
}

//--------------------------------------------------------------

- (void) stopLocation {
	[locationManager stopUpdatingLocation];
}

//--------------------------------------------------------------

- (bool) startMonitoringSignificantLocationChanges {
    if ([CLLocationManager locationServicesEnabled]) {
		[locationManager startMonitoringSignificantLocationChanges];
		return true;
	}
    return false;
}

//--------------------------------------------------------------

- (void) stopMonitoringSignificantLocationChanges {
  [locationManager stopMonitoringSignificantLocationChanges];
}

//--------------------------------------------------------------

// Called when the location is updated
- (void)locationManager:(CLLocationManager *)manager
	didUpdateToLocation:(CLLocation *)newLocation
		   fromLocation:(CLLocation *)oldLocation
{
	if (signbit(newLocation.horizontalAccuracy)) {
		// Negative accuracy means an invalid or unavailable measurement
		NSLog(@"LatLongUnavailable");
	} else {
		lat = newLocation.coordinate.latitude;
		lng = newLocation.coordinate.longitude;
		hAccuracy = newLocation.horizontalAccuracy;
		
		if (oldLocation != nil) {
			CLLocationDistance distanceMoved = [newLocation distanceFromLocation:oldLocation];
			distMoved = distanceMoved;
		}
	}

	if (signbit(newLocation.verticalAccuracy)) {
		// Negative accuracy means an invalid or unavailable measurement
		NSLog(@"AltUnavailable");
	} else {
		vAccuracy = newLocation.verticalAccuracy;
		alt = newLocation.altitude;
	}
	
}

//--------------------------------------------------------------

//called when the heading is updated
- (void)locationManager:(CLLocationManager *)manager didUpdateHeading:(CLHeading *)newHeading
{
	x = newHeading.x;
	y = newHeading.y;
	z = newHeading.z;
	magneticHeading = newHeading.magneticHeading;
	trueHeading = newHeading.trueHeading;
	headingAccuracy = newHeading.headingAccuracy;
}

//--------------------------------------------------------------

- (void)locationManager:(CLLocationManager *)manager
	   didFailWithError:(NSError *)error //thx apple
{
	NSMutableString *errorString = [[NSMutableString alloc] init];
	
	if ([error domain] == kCLErrorDomain) {
		
		// We handle CoreLocation-related errors here
		
		switch ([error code]) {
				// This error code is usually returned whenever user taps "Don't Allow" in response to
				// being told your app wants to access the current location. Once this happens, you cannot
				// attempt to get the location again until the app has quit and relaunched.
				//
				// "Don't Allow" on two successive app launches is the same as saying "never allow". The user
				// can reset this for all apps by going to Settings > General > Reset > Reset Location Warnings.
				//
			case kCLErrorDenied:
				[errorString appendFormat:@"%@\n", NSLocalizedString(@"LocationDenied", nil)];
				break;
				
				// This error code is usually returned whenever the device has no data or WiFi connectivity,
				// or when the location cannot be determined for some other reason.
				//
				// CoreLocation will keep trying, so you can keep waiting, or prompt the user.
				//
			case kCLErrorLocationUnknown:
				[errorString appendFormat:@"%@\n", NSLocalizedString(@"LocationUnknown", nil)];
				break;
				
				// We shouldn't ever get an unknown error code, but just in case...
				//
			default:
				[errorString appendFormat:@"%@ %i\n", NSLocalizedString(@"GenericLocationError", nil), (int)[error code]];
				break;
		}
	} else {
		// We handle all non-CoreLocation errors here
		// (we depend on localizedDescription for localization)
		[errorString appendFormat:@"Error domain: \"%@\"  Error code: %i\n", [error domain], (int)[error code]];
		[errorString appendFormat:@"Description: \"%@\"\n", [error localizedDescription]];
	}
	
	// Send the update to our delegate
	NSLog(@"%@", errorString);
}

@end

#endif
