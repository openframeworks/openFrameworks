#include "ContourFinder.h"

namespace ofxCv {
	using namespace cv;
	
	// it would be better if these were shallow copies
	// but ofPolyline is 3d while cv::Point is 2d
	ofPolyline toOf(vector<cv::Point>& contour) {
		ofPolyline polyline;
		polyline.resize(contour.size());
		for(int i = 0; i < contour.size(); i++) {
			polyline[i].x = contour[i].x;
			polyline[i].y = contour[i].y;
		}
		polyline.close();
		return polyline;
	}
	
	vector<cv::Point> toCv(ofPolyline& polyline) {
		vector<cv::Point> contour(polyline.size());
		for(int i = 0; i < polyline.size(); i++) {
			contour[i].x = polyline[i].x;
			contour[i].y = polyline[i].y;
		}
		return contour;		
	}
	
	ContourFinder::ContourFinder()
	:autoThreshold(true)
	,thresholdValue(128.)
	,invert(false)
	,simplify(true) {
		resetMinArea();
		resetMaxArea();
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
		for(int i = 0; i < contours.size(); i++) {
			polylines.push_back(toOf(contours[i]));
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
	
	void ContourFinder::resetMinArea() {
		minArea = 0;
	}
	
	void ContourFinder::resetMaxArea() {
		maxArea = numeric_limits<float>::infinity();
	}
	
	void ContourFinder::setMinArea(float minArea) {
		this->minArea = minArea;
		minAreaNorm = false;
	}
	
	void ContourFinder::setMaxArea(float maxArea) {
		this->maxArea = maxArea;
		maxAreaNorm = false;
	}
	
	void ContourFinder::setMinAreaRadius(float minAreaRadius) {
		minArea = PI * minAreaRadius * minAreaRadius;
		minAreaNorm = false;
	}
	
	void ContourFinder::setMaxAreaRadius(float maxAreaRadius) {
		maxArea = PI * maxAreaRadius * maxAreaRadius;
		maxAreaNorm = false;
	}
	
	void ContourFinder::setMinAreaNorm(float minAreaNorm) {
		minArea = minAreaNorm;
		this->minAreaNorm = minAreaNorm;
	}
	
	void ContourFinder::setMaxAreaNorm(float maxAreaNorm) {
		maxArea = maxAreaNorm;
		this->maxAreaNorm = maxAreaNorm;
	}
	
}
