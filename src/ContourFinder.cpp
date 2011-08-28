#include "ContourFinder.h"

namespace ofxCv {
	using namespace cv;
	
	ContourFinder::ContourFinder()
	:autoThreshold(true)
	,thresholdValue(128.)
	,simplify(true) {
	}
	
	void ContourFinder::findContours(Mat img) {
		if(autoThreshold) {
			if(img.channels() == 1) {
				thresh = img.clone();
			} else if(img.channels() == 3) {
				cvtColor(img, thresh, CV_RGB2GRAY);
			} else if(img.channels() == 4) {
				cvtColor(img, thresh, CV_RGBA2GRAY);
			}
			threshold(thresh, thresholdValue);
		} else {
			thresh = img;
		}
		
		contours.clear();
		int simplifyMode = simplify ? CV_CHAIN_APPROX_SIMPLE : CV_CHAIN_APPROX_NONE;
		cv::findContours(thresh, contours, CV_RETR_EXTERNAL, simplifyMode);
	}
	
	void ContourFinder::setAutoThreshold(bool autoThreshold) {
		this->autoThreshold = autoThreshold;
	}
	
	void ContourFinder::setThreshold(float thresholdValue) {
		this->thresholdValue = thresholdValue;
	}
	
	void ContourFinder::setSimplify(bool simplify) {
		this->simplify = simplify;
	}
	
	void ContourFinder::draw() {
		for(int i = 0; i < contours.size(); i++) {
			ofNoFill();
			ofBeginShape();
			for(int j = 0; j < contours[i].size(); j++) {
				ofVertex(contours[i][j].x, contours[i][j].y);
			}
			ofEndShape(true);
		}
	}
}
