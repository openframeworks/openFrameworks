#pragma once

#ifdef MIN 
	#undef MIN
#endif 
#ifdef MAX 
	#undef MAX 
#endif 

#ifdef USE_OLD_CV
    #include "cv.h"
    #include "opencv2/opencv.hpp"
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
