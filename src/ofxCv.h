/*
 there are three types of functions in the ofxCv namespace:
 1 low-level utility functions like imitate and toCv (Utilities.h)
 2 wrapper functions that accept toCv-compatible objects (Wrappers.h)
 3 helper functions that handle more complex tasks (Helpers.h)
 
 also in the namespace are a few helper classes that make common tasks easier.
 for example: camera calibration (Calibration) and contour finding
 (ContourFinder.h).
 
 all functions guarantee the size of the output with imitate when possible. data
 is returned using arguments when an expensive copy would be required, and using
 a return value when the data is small.
 */

#pragma once

// cv
#include "opencv2/opencv.hpp"

// ofxCv
#include "Utilities.h"
#include "Wrappers.h"
#include "Helpers.h"

#include "Distance.h"
#include "Calibration.h"
#include "ContourFinder.h"
#include "Tracker.h"

using namespace ofxCv;
using namespace cv;

// <3 kyle
