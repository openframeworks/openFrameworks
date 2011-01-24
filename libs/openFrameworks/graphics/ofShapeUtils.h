#pragma once

#include "ofShape.h"

// todo: this should be gaussian with a single width parameter
// todo: this should handle non-loop polylines correctly
// given a polyline, smooth all the points using a linear dropoff kernel
// of radius smoothingSize and weighted towards the edges with smoothingAmount
// smoothingAmount of 1 will give a moving average, while 0 gives a triangular kernel
ofPolyline ofGetSmoothed(const ofPolyline& polyline, int smoothingSize = 4, float smoothingAmount = 0);

// resample a polyline based on the distance between the points
ofPolyline ofGetResampledSpacing(const ofPolyline& polyline, float spacing = 4);

// resample a polyline based on the total point count
ofPolyline ofGetResampledCount(const ofPolyline& polyline, int count);

// todo: douglas-peucker simplification

// get the bounding box of a polyline
ofRectangle ofGetBoundingBox(const ofPolyline& polyline);

// todo: interesection, offset

/*

// find the closest point to p3 on the line between p1 and p2
// optionally pass a pointer to a float for the normalized position along that line
// also, helper functions for finding the closest point on a polyline or closest in a vector of polylines
ofPoint ofGetClosestPoint(const ofPoint& p1, const ofPoint& p2, const ofPoint& p3, float* uptr = NULL);
ofPoint ofGetClosestPoint(const ofPolyline& polyline, const ofPoint& target, int& nearest);
ofPoint ofGetClosestPoint(vector<ofPolyline>& polylines, const ofPoint& target, ofPolyline*& matchedPolyline, int& matchedIndex);
*/