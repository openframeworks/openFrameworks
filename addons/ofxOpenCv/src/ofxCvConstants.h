#pragma once

#ifdef MIN 
	#undef MIN
#endif 
#ifdef MAX 
	#undef MAX 
#endif 

#include "cv.h"
#include <vector>
#include "ofMain.h"

enum ofxCvRoiMode {
    OFX_CV_ROI_MODE_INTERSECT,
    OFX_CV_ROI_MODE_NONINTERSECT
};
