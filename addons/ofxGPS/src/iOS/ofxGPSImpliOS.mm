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

const double HORIZONTAL_ACCURACY_THRESHOLD = 30.0;
const double DISTANCE_FILTER = 2.0;
//const double DISTANCE_FILTER = kCLDistanceFilterNone;

bool ofxGPS::m_locationStarted = false;
bool ofxGPS::m_headingStarted= false;

ofEvent<const ofxGPS::LocationData> ofxGPS::newLocationDataEvent;
ofEvent<const ofxGPS::HeadingData> ofxGPS::newHeadingDataEvent;

ofxGPSImpliOSCoreLocationDelegate* coreLoc = NULL;

ofxGPSImpliOSCoreLocationDelegate* getCoreLocSafe() {
    
    if (!coreLoc)
        coreLoc = [[ofxGPSImpliOSCoreLocationDelegate alloc] init];
    
    return coreLoc;
}

bool isMonitoring = false;

void startMonitoring() {
    
    if (!isMonitoring) {
        [getCoreLocSafe() startMonitoringSignificantLocationChanges];
        isMonitoring= true;
    }
}

void releaseCoreLocIfNeeded() {
    
    if (!ofxGPS::headingStarted() && !ofxGPS::locationStarted()) {
        [coreLoc release];
        coreLoc = nil;
        isMonitoring= false;
    }
}

//--------------------------------------------------------------

bool ofxGPS::startHeading()
{
    bool result = [getCoreLocSafe() startHeading];
    
    if (result) {
        m_headingStarted = true;
    }
    
    return result;
}

//--------------------------------------------------------------

void ofxGPS::stopHeading()
{
    [getCoreLocSafe() stopHeading];
    
    m_headingStarted = false;
    
    releaseCoreLocIfNeeded();
}

//--------------------------------------------------------------
bool ofxGPS::startLocation()
{
	bool result = [getCoreLocSafe() startLocation];
    
    if (result) {
        m_locationStarted = true;
    }
    
    return result;
}

//--------------------------------------------------------------
void ofxGPS::stopLocation()
{
	[getCoreLocSafe() stopLocation];
    
    m_locationStarted = false;
    
    releaseCoreLocIfNeeded();
}

//--------------------------------------------------------------

// CLASS IMPLEMENTATIONS--------------objc------------------------
//----------------------------------------------------------------
@implementation ofxGPSImpliOSCoreLocationDelegate

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
        
        if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 9.0) {
            locationManager.allowsBackgroundLocationUpdates = YES;
        }
	}
	return self;
}

//--------------------------------------------------------------
- (void)dealloc 
{ 
	[locationManager release];
    locationManager = nil;
	
	[super dealloc];
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
        m_didReportLocation = false;
        
        locationManager.desiredAccuracy = kCLLocationAccuracyBestForNavigation;
        locationManager.distanceFilter = DISTANCE_FILTER;
        
        if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 8.0) {
            [locationManager requestAlwaysAuthorization];
        }
        
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
    ofxGPS::LocationData locationData;
    
	if (signbit(newLocation.horizontalAccuracy)) {
		// Negative accuracy means an invalid or unavailable measurement
		NSLog(@"LatLongUnavailable");
        locationData.hasLocation = false;
	}
    if (m_didReportLocation && newLocation.horizontalAccuracy > HORIZONTAL_ACCURACY_THRESHOLD) {
        // not accurate enough. Don't report this unless it's the first report.
        NSLog(@"LatLong not accurate");
        locationData.hasLocation = false;
    }
    else
    {
		lat = newLocation.coordinate.latitude;
		lng = newLocation.coordinate.longitude;
		hAccuracy = newLocation.horizontalAccuracy;
		
		if (oldLocation != nil) {
			CLLocationDistance distanceMoved = [newLocation distanceFromLocation:oldLocation];
			distMoved = distanceMoved;
		}
        
        locationData.hasLocation = true;
        locationData.latitude = lat;
        locationData.longitude = lng;
        locationData.locationAccuracy = hAccuracy;
        
        m_didReportLocation = true;
	}

	if (signbit(newLocation.verticalAccuracy)) {
		// Negative accuracy means an invalid or unavailable measurement
		NSLog(@"AltUnavailable");
        locationData.hasAltitude = false;
	} else {
		vAccuracy = newLocation.verticalAccuracy;
		alt = newLocation.altitude;
        
        locationData.hasAltitude = true;
        locationData.altitude = alt;
        locationData.altitudeAccuracy = vAccuracy;
	}
    
    ofNotifyEvent(ofxGPS::newLocationDataEvent, locationData);
}

//--------------------------------------------------------------

#ifdef __IPHONE_3_0
//called when the heading is updated
- (void)locationManager:(CLLocationManager *)manager didUpdateHeading:(CLHeading *)newHeading
{
    ofxGPS::HeadingData headingData;
    
    headingData.hasHeading = true;
	x = newHeading.x;
	y = newHeading.y;
	z = newHeading.z;
	magneticHeading = newHeading.magneticHeading;
	trueHeading = newHeading.trueHeading;
	headingAccuracy = newHeading.headingAccuracy;
    
    headingData.heading = 360.0 - trueHeading;
    headingData.headingAccuracy = headingAccuracy;
    
    ofNotifyEvent(ofxGPS::newHeadingDataEvent, headingData);
}

//- (BOOL)locationManagerShouldDisplayHeadingCalibration:(CLLocationManager *)manager {
//    return YES;
//}

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