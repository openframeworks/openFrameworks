#pragma once

// cv
#include "opencv2/opencv.hpp"

// ofxCv

// there are three types of functions in the ofxCv namespace
#include "ofxCv/Utilities.h" // low-level utilities like imitate and toCv
#include "ofxCv/Wrappers.h" // wrappers that accept toCv-compatible objects
#include "ofxCv/Helpers.h" // helper functions that handle more complex tasks

/*
 all functions guarantee the size of the output with imitate when possible. data
 is returned using arguments when an expensive copy would be required or when
 you want to use a preallocated buffer, and a return value is used when the data
 is small or there is probably no preallocated buffer.
 */

// also in the namespace are a few helper classes that make common tasks easier:
#include "ofxCv/Distance.h" // edit distance
#include "ofxCv/Calibration.h" // camera calibration
#include "ofxCv/Tracker.h" // object tracking
#include "ofxCv/ContourFinder.h" // contour finding and tracking
#include "ofxCv/RunningBackground.h" // background subtraction
#include "ofxCv/Flow.h" // optical flow, from james george
#include "ofxCv/ObjectFinder.h" // object finding (e.g., face detection)

// <3 kyle
