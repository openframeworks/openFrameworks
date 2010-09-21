/*
 *  ofxiPhoneCoreLocation.cpp
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

#import "ofxiPhoneCoreLocation.h"

//C++ class implementations

//--------------------------------------------------------------
ofxiPhoneCoreLocation::ofxiPhoneCoreLocation()
{
	coreLoc = [[ofxiPhoneCoreLocationDelegate alloc] init];
}

//--------------------------------------------------------------
ofxiPhoneCoreLocation::~ofxiPhoneCoreLocation()
{
	[coreLoc release];
}

//--------------------------------------------------------------

bool ofxiPhoneCoreLocation::startHeading()
{
	return [coreLoc startHeading];
}

//--------------------------------------------------------------

void ofxiPhoneCoreLocation::stopHeading()
{
	[coreLoc stopHeading];
}

//--------------------------------------------------------------
bool ofxiPhoneCoreLocation::startLocation()
{
	return [coreLoc startLocation];
}

//--------------------------------------------------------------
void ofxiPhoneCoreLocation::stopLocation()
{
	[coreLoc stopLocation];
}

//--------------------------------------------------------------

double ofxiPhoneCoreLocation::getLatitude()
{
	return [coreLoc lat];
}

//--------------------------------------------------------------
double ofxiPhoneCoreLocation::getLongitude()
{
	return [coreLoc lng];
}

//--------------------------------------------------------------
double ofxiPhoneCoreLocation::getLocationAccuracy()
{
	return [coreLoc hAccuracy];
}

//--------------------------------------------------------------
double ofxiPhoneCoreLocation::getAltitude()
{
	return [coreLoc alt];
}

//--------------------------------------------------------------
double ofxiPhoneCoreLocation::getAltitudeAccuracy()
{
	return [coreLoc vAccuracy];
}

//--------------------------------------------------------------
double ofxiPhoneCoreLocation::getDistMoved()
{
	return [coreLoc distMoved];
}

//--------------------------------------------------------------
double ofxiPhoneCoreLocation::getCompassX()
{
	return [coreLoc x];
}

//--------------------------------------------------------------
double ofxiPhoneCoreLocation::getCompassY()
{
	return [coreLoc y];
}

//--------------------------------------------------------------
double ofxiPhoneCoreLocation::getCompassZ()
{
	return [coreLoc z];
}

//--------------------------------------------------------------
double ofxiPhoneCoreLocation::getMagneticHeading()
{
	return [coreLoc magneticHeading];
}

//--------------------------------------------------------------
double ofxiPhoneCoreLocation::getTrueHeading()
{
	return [coreLoc trueHeading];
}

//--------------------------------------------------------------
double ofxiPhoneCoreLocation::getHeadingAccuracy()
{
	return [coreLoc headingAccuracy];
}

//--------------------------------------------------------------

// CLASS IMPLEMENTATIONS--------------objc------------------------
//----------------------------------------------------------------
@implementation ofxiPhoneCoreLocationDelegate

//--------------------------------------------------------------
//create getter/setter functions for these variables
@synthesize lat, lng, hAccuracy, alt, vAccuracy, distMoved, x, y, z, magneticHeading, trueHeading, headingAccuracy;

//--------------------------------------------------------------
- (id) init
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
	[locationManager release];
	
	[super dealloc];
}

//--------------------------------------------------------------

- (bool) startHeading
{
	if([locationManager headingAvailable])
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
	if([locationManager locationServicesEnabled])
	{
		[locationManager startUpdatingLocation];
		return true;
	}
	else
		return false;
	
}

//--------------------------------------------------------------

- (void) stopLocation
{
	[locationManager stopUpdatingLocation];
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
			CLLocationDistance distanceMoved = [newLocation getDistanceFrom:oldLocation];
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

#ifdef __IPHONE_3_0
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
#endif

//--------------------------------------------------------------

- (void)locationManager:(CLLocationManager *)manager
	   didFailWithError:(NSError *)error //thx apple
{
	NSMutableString *errorString = [[[NSMutableString alloc] init] autorelease];
	
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
				[errorString appendFormat:@"%@ %d\n", NSLocalizedString(@"GenericLocationError", nil), [error code]];
				break;
		}
	} else {
		// We handle all non-CoreLocation errors here
		// (we depend on localizedDescription for localization)
		[errorString appendFormat:@"Error domain: \"%@\"  Error code: %d\n", [error domain], [error code]];
		[errorString appendFormat:@"Description: \"%@\"\n", [error localizedDescription]];
	}
	
	// Send the update to our delegate
	NSLog(@"%@", errorString);
}

@end