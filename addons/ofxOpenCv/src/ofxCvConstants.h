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
