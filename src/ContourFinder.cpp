#include "ContourFinder.h"

namespace ofxCv {
	using namespace cv;
	
	ContourFinder::ContourFinder()
	:autoThreshold(true)
	,thresholdValue(128.)
	,invert(false)
	,simplify(true) {
	}
	
	void ContourFinder::findContours(Mat img) {
		if(img.channels() == 1) {
			thresh = img.clone();
		} else if(img.channels() == 3) {
			cvtColor(img, thresh, CV_RGB2GRAY);
		} else if(img.channels() == 4) {
			cvtColor(img, thresh, CV_RGBA2GRAY);
		}
		if(autoThreshold) {
			threshold(thresh, thresholdValue, invert);
		}
				
		contours.clear();
		int simplifyMode = simplify ? CV_CHAIN_APPROX_SIMPLE : CV_CHAIN_APPROX_NONE;
		cv::findContours(thresh, contours, CV_RETR_EXTERNAL, simplifyMode);
		
		polylines.clear();
		polylines.resize(contours.size());
		for(int i = 0; i < contours.size(); i++) {
			for(int j = 0; j < contours[i].size(); j++) {
				polylines[i].addVertex(toOf(contours[i][j]));
			}
			polylines[i].close();
		}
	}
	
	vector<vector<cv::Point> >& ContourFinder::getContours() {
		return contours;
	}
	
	vector<ofPolyline>& ContourFinder::getPolylines() {
		return polylines;
	}
	
	void ContourFinder::setAutoThreshold(bool autoThreshold) {
		this->autoThreshold = autoThreshold;
	}
	
	void ContourFinder::setThreshold(float thresholdValue) {
		this->thresholdValue = thresholdValue;
	}
	
	void ContourFinder::setInvert(bool invert) {
		this->invert = invert;
	}
	
	void ContourFinder::setSimplify(bool simplify) {
		this->simplify = simplify;
	}
	
	void ContourFinder::draw() {
		for(int i = 0; i < polylines.size(); i++) {
			polylines[i].draw();
		}
	}
}
