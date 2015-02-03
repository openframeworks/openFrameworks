/*
 *
 *  ofxGPSImpliOS.mm
 *
 *  Created by Tal Lavi on 10/2/14.
 *  Based on ofxiOSCoreLocation.cpp by Zach Gage
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

#import "ofxGPSImpliOS.h"

//C++ class implementations

ofEvent<const ofxGPSData> ofxGPS::gpsDataChangedEvent;

//--------------------------------------------------------------
ofxGPSImpliOS::ofxGPSImpliOS()
{
	coreLoc = [[ofxGPSImpliOSCoreLocationDelegate alloc] init];
    
    [coreLoc startLocation];
    [coreLoc startHeading];
    [coreLoc startMonitoringSignificantLocationChanges];
}

//--------------------------------------------------------------
ofxGPSImpliOS::~ofxGPSImpliOS()
{
	[coreLoc release];
}

ofxGPSData ofxGPSImpliOS::getGPSData()
{
    return [coreLoc gpsData];
}

//--------------------------------------------------------------

bool ofxGPSImpliOS::startHeading()
{
	return [coreLoc startHeading];
}

//--------------------------------------------------------------

void ofxGPSImpliOS::stopHeading()
{
	[coreLoc stopHeading];
}

//--------------------------------------------------------------
bool ofxGPSImpliOS::startLocation()
{
	return [coreLoc startLocation];
}

//--------------------------------------------------------------
void ofxGPSImpliOS::stopLocation()
{
	[coreLoc stopLocation];
}

//--------------------------------------------------------------
bool ofxGPSImpliOS::startMonitoringSignificantLocationChanges() {
  return [coreLoc startMonitoringSignificantLocationChanges];
}

//--------------------------------------------------------------
void ofxGPSImpliOS::stopMonitoringSignificantLocationChanges() {
  [coreLoc stopMonitoringSignificantLocationChanges];
}

//--------------------------------------------------------------

std::shared_ptr<ofxGPS> ofxGPS::create()
{
    return std::shared_ptr<ofxGPS>(new ofxGPSImpliOS());
}

//--------------------------------------------------------------

// CLASS IMPLEMENTATIONS--------------objc------------------------
//----------------------------------------------------------------
@implementation ofxGPSImpliOSCoreLocationDelegate

//--------------------------------------------------------------
//create getter/setter functions for these variables
@synthesize lat, lng, hAccuracy, alt, vAccuracy, distMoved, x, y, z, magneticHeading, trueHeading, headingAccuracy, gpsData;

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
        
        gpsData.hasLocation = false;
        gpsData.hasAltitude = false;
        gpsData.hasHeading = false;
		
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
	if([CLLocationManager headingAvailable])
	{
        [locationManager requestAlwaysAuthorization];
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
    gpsData.time = Poco::Timestamp();
    
	if (signbit(newLocation.horizontalAccuracy)) {
		// Negative accuracy means an invalid or unavailable measurement
		NSLog(@"LatLongUnavailable");
        gpsData.hasLocation = false;
	} else {
		lat = newLocation.coordinate.latitude;
		lng = newLocation.coordinate.longitude;
		hAccuracy = newLocation.horizontalAccuracy;
		
		if (oldLocation != nil) {
			CLLocationDistance distanceMoved = [newLocation distanceFromLocation:oldLocation];
			distMoved = distanceMoved;
		}
        
        gpsData.hasLocation = true;
        gpsData.latitude = lat;
        gpsData.longitude = lng;
        gpsData.locationAccuracy = hAccuracy;
	}

	if (signbit(newLocation.verticalAccuracy)) {
		// Negative accuracy means an invalid or unavailable measurement
		NSLog(@"AltUnavailable");
        gpsData.hasHeading = false;
	} else {
		vAccuracy = newLocation.verticalAccuracy;
		alt = newLocation.altitude;
        
        gpsData.hasAltitude = true;
        gpsData.altitude = alt;
        gpsData.headingAccuracy = vAccuracy;
	}
	
    ofNotifyEvent(ofxGPS::gpsDataChangedEvent, gpsData);
}

//--------------------------------------------------------------

#ifdef __IPHONE_3_0
//called when the heading is updated
- (void)locationManager:(CLLocationManager *)manager didUpdateHeading:(CLHeading *)newHeading
{
    gpsData.hasHeading = true;
	x = newHeading.x;
	y = newHeading.y;
	z = newHeading.z;
	magneticHeading = newHeading.magneticHeading;
	trueHeading = newHeading.trueHeading;
	headingAccuracy = newHeading.headingAccuracy;
    
    gpsData.heading = 360.0 - trueHeading;
    gpsData.headingAccuracy = headingAccuracy;
    
    ofNotifyEvent(ofxGPS::gpsDataChangedEvent, gpsData);
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