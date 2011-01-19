#pragma once

#include "ofShape.h"

// todo: these should return objects instead of doing it faux-in-place

// todo: this should be gaussian with a single width parameter
// todo: this should handle non-loop polylines correctly
// given a polyline, smooth all the points using a linear dropoff kernel
// of radius smoothingSize and weighted towards the edges with smoothingAmount
// smoothingAmount of 1 will give a moving average, while 0 gives a triangular kernel
void ofSmoothPolyline(ofPolyline& polyline, int smoothingSize = 8, float smoothingAmount = 0);

// evenly resample a polyline with equally spaced points.
void ofResamplePolyline(ofPolyline& polyline, float spacing = 4);

// todo: fixed number of points resampling
// todo: douglas-pecker simplification
// todo: get bounding box from polyline
// todo: interesection, offset

/*

// find the closest point to p3 on the line between p1 and p2
// optionally pass a pointer to a float for the normalized position along that line
// also, helper functions for finding the closest point on a polyline or closest in a vector of polylines
ofPoint ofGetClosestPoint(const ofPoint& p1, const ofPoint& p2, const ofPoint& p3, float* uptr = NULL);
ofPoint ofGetClosestPoint(const ofPolyline& polyline, const ofPoint& target, int& nearest);
ofPoint ofGetClosestPoint(vector<ofPolyline>& polylines, const ofPoint& target, ofPolyline*& matchedPolyline, int& matchedIndex);

// get a point on a polyline that is a distance in pixels from a given point
void getOffsetPoint(ofPolyline& polyline, int start, float radius, int& lastIndex, ofPoint& point);

*/