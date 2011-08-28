#include "ContourFinder.h"

namespace ofxCv {
		ContourFinder::ContourFinder()
		:autoThreshold(true)
		,thresholdValue(128.) {
		}
		
		void ContourFinder::findContours(Mat img) {
			if(autoThreshold) {
				if(img.channels() == 1) {
					thresh = img.clone();
				} else {
					convertColor(img, thresh, CV_RGB2GRAY);
				}
				ofxCv::threshold(thresh, thresholdValue);
			} else {
				thresh = toCv(img);
			}
		}
		
		void ContourFinder::setAutoThreshold(bool autoThreshold) {
			this->autoThreshold = autoThreshold;
		}
		
		void ContourFinder::setThreshold(float thresholdValue) {
			this->thresholdValue = thresholdValue;
		}	
}
