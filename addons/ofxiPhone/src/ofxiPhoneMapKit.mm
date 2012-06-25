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

#include <Availability.h>
#ifdef __IPHONE_3_0

#include "ofxiPhoneMapKit.h"
#include "ofxiPhoneExtras.h"
#include "ofxiPhoneMapKitDelegate.h"

ofxiPhoneMapKit::ofxiPhoneMapKit() {
	mapView = nil;
}

ofxiPhoneMapKit::~ofxiPhoneMapKit() {
	close();
}


void ofxiPhoneMapKit::open() {
	ofLog(OF_LOG_VERBOSE, "ofxiPhoneMapKit::open()");
	if(!mapView) {
		ofLog(OF_LOG_VERBOSE, "   alloc MKMapView");
		mapView	= [[MKMapView alloc] initWithFrame:CGRectMake(0, 0, ofGetWidth(), ofGetHeight())];
		mapView.delegate = nil;
        [ofxiPhoneGetUIWindow() addSubview:mapView];
	}
}


void ofxiPhoneMapKit::close() {
	ofLog(OF_LOG_VERBOSE, "ofxiPhoneMapKit::close()");
    if(mapView) {
        mapView.delegate = nil;
        [mapView removeFromSuperview];
        [mapView release];
        mapView = nil;
    }
}

bool ofxiPhoneMapKit::isOpen() {
    return (mapView != nil);
}


void ofxiPhoneMapKit::setCenter(double latitude, double longitude, bool animated) {
	CLLocationCoordinate2D center = makeCLLocation(latitude, longitude);
	[mapView setCenterCoordinate:center animated:animated];
}


void ofxiPhoneMapKit::setSpan(double latitudeDelta, double longitudeDelta, bool animated) {
	_setRegion(mapView.region.center, makeMKCoordinateSpan(latitudeDelta, longitudeDelta), animated);
}

void ofxiPhoneMapKit::setSpanWithMeters(double metersLatitude, double metersLongitude, bool animated) {
	CLLocationCoordinate2D currentCenter	= mapView.region.center;
	_setRegion(currentCenter, MKCoordinateRegionMakeWithDistance(currentCenter, metersLatitude, metersLongitude).span, animated);
}

void ofxiPhoneMapKit::setRegion(double latitude, double longitude, double latitudeDelta, double longitudeDelta, bool animated) {
	_setRegion( makeCLLocation(latitude, longitude), makeMKCoordinateSpan(latitudeDelta, longitudeDelta), animated);
}


void ofxiPhoneMapKit::setRegionWithMeters(double latitude, double longitude, double metersLatitude, double metersLongitude, bool animated) {
	CLLocationCoordinate2D newCenter = makeCLLocation(latitude, longitude);
	_setRegion(newCenter, MKCoordinateRegionMakeWithDistance(newCenter, metersLatitude, metersLongitude).span, animated);
}


void ofxiPhoneMapKit::_setRegion(CLLocationCoordinate2D center, MKCoordinateSpan span, bool animated) {
	MKCoordinateRegion currentRegion = { center, span };
	[mapView setRegion:currentRegion animated:animated];
}

void ofxiPhoneMapKit::setType(ofxiPhoneMapKitType type) {
	ofLog(OF_LOG_VERBOSE, "ofxiPhoneMapKit::setType");
	mapView.mapType = type;
}

void ofxiPhoneMapKit::setShowUserLocation(bool b) {
	mapView.showsUserLocation = b;
}

void ofxiPhoneMapKit::setAllowUserInteraction(bool b) {
	mapView.userInteractionEnabled = b;
}

void ofxiPhoneMapKit::setAllowZoom(bool b) {
	mapView.zoomEnabled = b;
}

void ofxiPhoneMapKit::setAllowScroll(bool b) {
	mapView.scrollEnabled = b;
}

bool ofxiPhoneMapKit::isUserOnScreen() {
	return mapView.userLocationVisible;
}


CLLocationCoordinate2D ofxiPhoneMapKit::getCenterLocation() {
	return mapView.centerCoordinate;
}


// convert location (latitude, longitude) to screen coordinates (i.e. pixels)
ofPoint ofxiPhoneMapKit::getScreenCoordinatesForLocation(double latitude, double longitude) {
	CGPoint cgPoint = [mapView convertCoordinate:makeCLLocation(latitude, longitude) toPointToView:nil];
	return ofPoint(cgPoint.x, cgPoint.y);
}


ofxMapKitLocation ofxiPhoneMapKit::getLocationForScreenCoordinates(float x, float y) {
	return [mapView convertPoint:CGPointMake(x, y) toCoordinateFromView:nil];
}



// convert location (latitude, longitude) and span (in degrees) to screen coordinates (i.e. pixels)
ofRectangle ofxiPhoneMapKit::getScreenRectForRegion(double latitude, double latitudeDelta, double longitudeDelta) {
	ofRectangle r;
	//	- (CGRect)convertRegion:(MKCoordinateRegion)region toRectToView:(UIView *)view
	//	CGRect	cgRect = [mapView convertRegion:
	return r;
}


// convert location (latitude, longitude) and span (in meters) to screen coordinates (i.e. pixels)
ofRectangle ofxiPhoneMapKit::getScreenRectForRegionWithMeters(double latitude, double longitude, double metersLatitude, double metersLongitude) {
	ofRectangle r;
	return r;
}




#define MAX_LATITUDE	89.999
#define MAX_LONGITUDE	179.999

CLLocationCoordinate2D ofxiPhoneMapKit::makeCLLocation(double latitude, double longitude) {
	CLLocationCoordinate2D center = { 
		CLAMP(latitude, -MAX_LATITUDE, MAX_LATITUDE),
		CLAMP(longitude, -MAX_LONGITUDE, MAX_LONGITUDE)		
	};
	return center;
}

MKCoordinateSpan ofxiPhoneMapKit::makeMKCoordinateSpan(double latitudeDelta, double longitudeDelta) {
	MKCoordinateSpan span = { latitudeDelta, longitudeDelta };
	return span;
}


void ofxiPhoneMapKit::addListener(ofxiPhoneMapKitListener* o) {
	ofLog(OF_LOG_VERBOSE, "ofxiPhoneMapKit::addListener()");
	
	if(mapView.delegate == nil) mapView.delegate = [[ofxiPhoneMapKitDelegate alloc] initWithMapKit:this];
	listeners.push_back(o);
}

void ofxiPhoneMapKit::removeListener(ofxiPhoneMapKitListener* o) {
	ofLog(OF_LOG_VERBOSE, "ofxiPhoneMapKit::removeListener()");
	listeners.remove(o);
}

void ofxiPhoneMapKit::regionWillChange(bool animated) {
	for(std::list<ofxiPhoneMapKitListener*>::iterator it=listeners.begin(); it!=listeners.end(); ++it) {
		ofxiPhoneMapKitListener* o = *it;
		o->regionWillChange(animated);
	}
}

void ofxiPhoneMapKit::regionDidChange(bool animated) {
	for(std::list<ofxiPhoneMapKitListener*>::iterator it=listeners.begin(); it!=listeners.end(); ++it) {
		ofxiPhoneMapKitListener* o = *it;
		o->regionDidChange(animated);
	}
}

void ofxiPhoneMapKit::willStartLoadingMap() {
	for(std::list<ofxiPhoneMapKitListener*>::iterator it=listeners.begin(); it!=listeners.end(); ++it) {
		ofxiPhoneMapKitListener* o = *it;
		o->willStartLoadingMap();
	}
}


void ofxiPhoneMapKit::didFinishLoadingMap() {
	for(std::list<ofxiPhoneMapKitListener*>::iterator it=listeners.begin(); it!=listeners.end(); ++it) {
		ofxiPhoneMapKitListener* o = *it;
		o->didFinishLoadingMap();
	}
}


void ofxiPhoneMapKit::errorLoadingMap(string errorDescription) {
	for(std::list<ofxiPhoneMapKitListener*>::iterator it=listeners.begin(); it!=listeners.end(); ++it) {
		ofxiPhoneMapKitListener* o = *it;
		o->errorLoadingMap(errorDescription);
	}
}


#endif
