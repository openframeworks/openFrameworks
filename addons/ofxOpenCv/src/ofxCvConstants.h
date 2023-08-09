#pragma once

#ifdef NO
    #undef NO
#endif
#ifdef MIN 
	#undef MIN
#endif 
#ifdef MAX 
	#undef MAX 
#endif

//fix for X11 Status clash with OpenCV2 Stitching.hpp
#ifdef Status
    #define Status_Save Status
    #undef Status
#endif

#include "opencv2/core/version.hpp"
#if CV_MAJOR_VERSION < 4
    #include "cv.h"
    #define USE_OLD_CV
#else
    #include "opencv2/opencv.hpp"
    #include "opencv2/imgproc/imgproc_c.h"
#endif

#include <vector>
#include "ofMain.h"

enum ofxCvRoiMode {
    OFX_CV_ROI_MODE_INTERSECT,
    OFX_CV_ROI_MODE_NONINTERSECT
};

#ifdef Status_Save
    #define Status Status_Save
#endif
