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

#include <Availability.h>
#ifdef __IPHONE_3_0


#include "ofMain.h"
#include <MapKit/MapKit.h>
#include "ofxiPhoneMapKitListener.h"
#include <list>

// these are the types you can set for the map
enum ofxiPhoneMapKitType {
	OFXIPHONE_MAPKIT_MAP		= MKMapTypeStandard,
	OFXIPHONE_MAPKIT_SATELLITE	= MKMapTypeSatellite,
	OFXIPHONE_MAPKIT_HYRBID		= MKMapTypeHybrid
};


// this is a type, similar to ofPoint, but with better precision for storing latitude and longitude
typedef CLLocationCoordinate2D ofxMapKitLocation;

class ofxiPhoneMapKit : public ofxiPhoneMapKitListener {
public:
	
	ofxiPhoneMapKit();
	virtual ~ofxiPhoneMapKit();
	
	
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
	
	// set the map type (see ofxiPhoneMapKitType above)
	void setType(ofxiPhoneMapKitType type = OFXIPHONE_MAPKIT_MAP);
	
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
	ofPoint getScreenCoordinatesForLocation(double latitude, double longitude);
	
	// convert screen coordinates (i.e. pixels) to location (latitude, longitude)
	ofxMapKitLocation getLocationForScreenCoordinates(float x, float y);

	
	
	// convert location (latitude, longitude) and span (in degrees) to screen coordinates (i.e. pixels)
	ofRectangle getScreenRectForRegion(double latitude, double latitudeDelta, double longitudeDelta); 
	
	
	// convert location (latitude, longitude) and span (in meters) to screen coordinates (i.e. pixels)
	ofRectangle getScreenRectForRegionWithMeters(double latitude, double longitude, double metersLatitude, double metersLongitude); 

	
	// returns whether the map is open or not
	bool isOpen();
	
	void addListener(ofxiPhoneMapKitListener* l);	
	void removeListener(ofxiPhoneMapKitListener* l);

	void regionWillChange(bool animated);
	void regionDidChange(bool animated);
	void willStartLoadingMap();
	void didFinishLoadingMap();
	void errorLoadingMap(string errorDescription);
	

	// return instance to MKMapView
	MKMapView	*getMKMapView();
	
protected:
	
	MKMapView	*mapView;
	std::list<ofxiPhoneMapKitListener*> listeners;
	
	CLLocationCoordinate2D makeCLLocation(double latitude, double longitude);
	MKCoordinateSpan makeMKCoordinateSpan(double latitudeDelta, double longitudeDelta);
	
	void _setRegion(CLLocationCoordinate2D center, MKCoordinateSpan span, bool animated);
};

#endif