/***********************************************************************
 
 Copyright (c) 2008, 2009, Memo Akten, www.memo.tv, Douglas Edric Stanley, www.abstractmachine.net
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of MSA Visuals nor the names of its contributors 
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE. 
 *
 * ***********************************************************************/ 

#pragma once

#include <TargetConditionals.h>
#if TARGET_OS_IOS || (TARGET_OS_IPHONE && !TARGET_OS_TV)

#import <MapKit/MapKit.h>
#include "ofConstants.h"
#include "ofxiOSMapKitListener.h"
#import "ofxiOSMapKitDelegate.h"
#include "glm/vec2.hpp"
#include "ofRectangle.h"
#include <list>

// these are the types you can set for the map
enum ofxiOSMapKitType {
    OFXIOS_MAPKIT_MAP           = MKMapTypeStandard,
    OFXIOS_MAPKIT_SATELLITE     = MKMapTypeSatellite,
    OFXIOS_MAPKIT_HYRBID        = MKMapTypeHybrid
};


// this is a type, similar to ofPoint, but with better precision for storing latitude and longitude
typedef CLLocationCoordinate2D ofxMapKitLocation;

class ofxiOSMapKit : public ofxiOSMapKitListener {
public:
	
	ofxiOSMapKit();
	virtual ~ofxiOSMapKit();
	
	
	// open the mapview
	void open();
	
	// hide the mapview
	void close();

	
	// latitude is south/north (-90...90)
	// longitude is east/west (-180...180)
	
	// set center (latitude,longitude) of map
	void setCenter(double latitude, double longitude, bool animated = true);
	
	// set span of map (in degrees)
	void setSpan(double latitudeDelta, double longitudeDelta, bool animated = true);
	
	// set span of map (in meters)
	void setSpanWithMeters(double metersLatitude, double metersLongitude, bool animated = true);

	// set center (latidude, longitude) and span (in degrees)
	void setRegion(double latitude, double longitude, double latitudeDelta, double longitudeDelta, bool animated = true);
	
	// set center (latidude, longitude) and span (in meters)
	void setRegionWithMeters(double latitude, double longitude, double metersLatitude, double metersLongitude, bool animated = true);
	
	// set the map type (see ofxiOSMapKitType above)
	void setType(ofxiOSMapKitType type = OFXIOS_MAPKIT_MAP);
	
	// set whether user location is visible on the map (as a blue dot)
	void setShowUserLocation(bool b);
	
	// enable/disable user interaction
	// if user interaction is not allowed, setAllowZoom / setAllowScroll are ignored
	// if user interaction is allowed, testApp::touchXXXX events will not be called
	void setAllowUserInteraction(bool b);

	// set whether user is allowed to zoom in or not
	void setAllowZoom(bool b);

	// set whether user is allowed to scroll the view or not
	void setAllowScroll(bool b);
	
	// returns whether the user location is visible in the current map region
	bool isUserOnScreen();
	
	// return current center of map (latitude, longitude)
	ofxMapKitLocation getCenterLocation();
	
	
	// convert location (latitude, longitude) to screen coordinates (i.e. pixels)
	glm::vec2 getScreenCoordinatesForLocation(double latitude, double longitude);
	
	// convert screen coordinates (i.e. pixels) to location (latitude, longitude)
	ofxMapKitLocation getLocationForScreenCoordinates(float x, float y);

	
	
	// convert location (latitude, longitude) and span (in degrees) to screen coordinates (i.e. pixels)
	ofRectangle getScreenRectForRegion(double latitude, double latitudeDelta, double longitudeDelta); 
	
	
	// convert location (latitude, longitude) and span (in meters) to screen coordinates (i.e. pixels)
	ofRectangle getScreenRectForRegionWithMeters(double latitude, double longitude, double metersLatitude, double metersLongitude); 

	
	// returns whether the map is open or not
	bool isOpen();
	
	void addListener(ofxiOSMapKitListener* l);	
	void removeListener(ofxiOSMapKitListener* l);

	void regionWillChange(bool animated);
	void regionDidChange(bool animated);
	void willStartLoadingMap();
	void didFinishLoadingMap();
	void errorLoadingMap(std::string errorDescription);
	

	// return instance to MKMapView
	MKMapView	*getMKMapView();
	
protected:
    ofxiOSMapKitDelegate *mapKitDelegate;
	MKMapView	*mapView;
	std::list<ofxiOSMapKitListener*> listeners;
	
	CLLocationCoordinate2D makeCLLocation(double latitude, double longitude);
	MKCoordinateSpan makeMKCoordinateSpan(double latitudeDelta, double longitudeDelta);
	
	void _setRegion(CLLocationCoordinate2D center, MKCoordinateSpan span, bool animated);
};

//-------------------------------------------------------------------------------
// backwards compatibility == 0.8.1
#define ofxiOS_MAPKIT_MAP           OFXIOS_MAPKIT_MAP
#define ofxiOS_MAPKIT_SATELLITE     OFXIOS_MAPKIT_SATELLITE
#define ofxiOS_MAPKIT_HYRBID        OFXIOS_MAPKIT_HYRBID
// backwards compatibility < 0.8.0
#define ofxiPhone_MAPKIT_MAP        OFXIOS_MAPKIT_MAP
#define ofxiPhone_MAPKIT_SATELLITE  OFXIOS_MAPKIT_SATELLITE
#define ofxiPhone_MAPKIT_HYRBID     OFXIOS_MAPKIT_HYRBID
#define ofxiPhoneMapKit ofxiOSMapKit
//-------------------------------------------------------------------------------

#endif

