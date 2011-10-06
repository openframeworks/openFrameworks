/*
 there are three types of functions in the ofxCv namespace:
 1 low-level utility functions like imitate and toCv (Utilities.h)
 2 wrapper functions that accept toCv-compatible objects (Wrappers.h)
 3 helper functions that handle more complex tasks (Helpers.h)
 
 also in the namespace are a few helper classes that make common tasks easier.
 for example: camera calibration (Calibration.h) and contour finding
 (ContourFinder.h).
 
 all functions guarantee the size of the output with imitate when possible. data
 is returned using arguments when an expensive copy would be required or when
 you want to use a preallocated buffer, and a return value is used when the data
 is small or there is probably no preallocated buffer.
 */

#pragma once

// cv
#include "opencv2/opencv.hpp"

// ofxCv
#include "ofxCv/Utilities.h"
#include "ofxCv/Wrappers.h"
#include "ofxCv/Helpers.h"

#include "ofxCv/Distance.h"
#include "ofxCv/Calibration.h"
#include "ofxCv/ContourFinder.h"
#include "ofxCv/Tracker.h"
#include "ofxCv/RunningBackground.h"

// <3 kyle
