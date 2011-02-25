#pragma once

#include "ofPolyline.h"
#include "ofRectangle.h"

// todo: douglas-peucker simplification

// todo: this should be gaussian with a single width parameter
// todo: this should handle non-loop polylines correctly
// given a polyline, smooth all the points using a linear dropoff kernel
// of radius smoothingSize and weighted towards the edges with smoothingAmount
// smoothingShape of 1 will give a moving average, while 0 gives a triangular kernel
ofPolyline ofGetSmoothed(const ofPolyline& polyline, int smoothingSize, float smoothingShape = 0);

// resample a polyline based on the distance between the points
ofPolyline ofGetResampledSpacing(const ofPolyline& polyline, float spacing);

// resample a polyline based on the total point count
ofPolyline ofGetResampledCount(const ofPolyline& polyline, int count);

// get the bounding box of a polyline
ofRectangle ofGetBoundingBox(const ofPolyline& polyline);

// find the closest point p3 on the line between p1 and p2
// optionally pass a pointer to/address of a float to get the normalized position along that line
ofPoint ofGetClosestPoint(const ofPoint& p1, const ofPoint& p2, const ofPoint& p3, float* normalizedPosition = NULL);

// find the closest point 'target' on a polyline
// optionally pass a pointer to/address of an unsigned int to get the index of the closest vertex
ofPoint ofGetClosestPoint(const ofPolyline& polyline, const ofPoint& target, unsigned int* nearestIndex = NULL);



