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
#endif
// OpenCV 4: imgproc_c.h. OpenCV 5: IplImage/cv* shims in ofxCvCompat.h.
#include "ofxCvCompat.h"

#include <vector>
#include "ofMain.h"

enum ofxCvRoiMode {
    OFX_CV_ROI_MODE_INTERSECT,
    OFX_CV_ROI_MODE_NONINTERSECT
};
