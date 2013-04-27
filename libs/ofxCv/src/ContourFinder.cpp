#include "ofxCv/ContourFinder.h"
#include "ofxCv/Wrappers.h"

namespace ofxCv {
	
	using namespace cv;
	
	ContourFinder::ContourFinder()
	:autoThreshold(true)
	,invert(false)
	,simplify(true)
	,thresholdValue(128.)
	,useTargetColor(false)
	,contourFindingMode(CV_RETR_EXTERNAL){
		resetMinArea();
		resetMaxArea();
	}
	
	void ContourFinder::findContours(Mat img) {
		// threshold the image using a tracked color or just binary grayscale
		if(useTargetColor) {
			Scalar offset(thresholdValue, thresholdValue, thresholdValue);
			Scalar base = toCv(targetColor);
			if(trackingColorMode == TRACK_COLOR_RGB) {
				inRange(img, base - offset, base + offset, thresh);
			} else {
				if(TRACK_COLOR_H) {
					offset[1] = 255;
					offset[2] = 255;
				}
				if(TRACK_COLOR_HS) {
					offset[2] = 255;
				}
				cvtColor(img, hsvBuffer, CV_RGB2HSV);
				base = toCv(convertColor(targetColor, CV_RGB2HSV));
				Scalar lowerb = base - offset;
				Scalar upperb = base + offset;
				inRange(hsvBuffer, lowerb, upperb, thresh);
			}
		} else {
			if(img.channels() == 1) {
				thresh = img.clone();
			} else if(img.channels() == 3) {
				cvtColor(img, thresh, CV_RGB2GRAY);
			} else if(img.channels() == 4) {
				cvtColor(img, thresh, CV_RGBA2GRAY);
			}
		}
		if(autoThreshold) {
			threshold(thresh, thresholdValue, invert);
		}
		
		// run the contour finder
		vector<vector<cv::Point> > allContours;
		int simplifyMode = simplify ? CV_CHAIN_APPROX_SIMPLE : CV_CHAIN_APPROX_NONE;
		cv::findContours(thresh, allContours, contourFindingMode, simplifyMode);
		
		// filter the contours
		bool needMinFilter = (minArea > 0);
		bool needMaxFilter = maxAreaNorm ? (maxArea < 1) : (maxArea < numeric_limits<float>::infinity());
		if(needMinFilter || needMaxFilter) {
			contours.clear();
			double imgArea = img.rows * img.cols;
			double imgMinArea = minAreaNorm ? (minArea * imgArea) : minArea;
			double imgMaxArea = maxAreaNorm ? (maxArea * imgArea) : maxArea;
			for(int i = 0; i < (int)allContours.size(); i++) {
				double curArea = contourArea(Mat(allContours[i]));
				if((!needMinFilter || curArea >= imgMinArea) &&
					 (!needMaxFilter || curArea <= imgMaxArea)) {
					contours.push_back(allContours[i]);
				}
			}
		} else {
			contours = allContours;
		}
		
		// generate polylines from the contours
		polylines.clear();
		for(int i = 0; i < (int)size(); i++) {
			polylines.push_back(toOf(contours[i]));
		}
		
		// generate bounding boxes from the contours
		boundingRects.clear();
		for(int i = 0; i < (int)size(); i++) {
			boundingRects.push_back(boundingRect(Mat(contours[i])));
		}
		
		// track bounding boxes
		tracker.track(boundingRects);
	}
	

	void ContourFinder::setFindHoles(bool findHoles){
		if(findHoles){
			contourFindingMode = CV_RETR_LIST;
		}else{
			contourFindingMode = CV_RETR_EXTERNAL;
		}
	}

	const vector<vector<cv::Point> >& ContourFinder::getContours() const {
		return contours;
	}
	
	const vector<ofPolyline>& ContourFinder::getPolylines() const {
		return polylines;
	}
	
	const vector<cv::Rect>& ContourFinder::getBoundingRects() const {
		return boundingRects;
	}
	
	unsigned int ContourFinder::size() const {
		return contours.size();
	}
	
	vector<cv::Point>& ContourFinder::getContour(unsigned int i) {
		return contours[i];
	}
	
	ofPolyline& ContourFinder::getPolyline(unsigned int i) {
		return polylines[i];
	}
	
	cv::Rect ContourFinder::getBoundingRect(unsigned int i) const {
		return boundingRects[i];
	}
	
	cv::Point2f ContourFinder::getCenter(unsigned int i) const {
		cv::Rect box = getBoundingRect(i);
		return cv::Point2f(box.x + box.width / 2, box.y + box.height / 2);
	}
	
	cv::Point2f ContourFinder::getCentroid(unsigned int i) const {
		Moments m = moments(Mat(contours[i]));
		return cv::Point2f(m.m10 / m.m00, m.m01 / m.m00);
	}
	
	cv::Point2f ContourFinder::getAverage(unsigned int i) const {
		Scalar average = mean(Mat(contours[i]));
		return cv::Point2f(average[0], average[1]);
	}
	
	cv::Vec2f ContourFinder::getBalance(unsigned int i) const {
		return cv::Vec2f(getCentroid(i) - getCenter(i));
	}
	
	double ContourFinder::getContourArea(unsigned int i) const {
		return contourArea(Mat(contours[i]));
	}
	
	double ContourFinder::getArcLength(unsigned int i) const {
		return arcLength(Mat(contours[i]), true);
	}
	
	vector<cv::Point> ContourFinder::getConvexHull(unsigned int i) const {
		vector<cv::Point> hull;
		convexHull(Mat(contours[i]), hull);
		return hull;
	}
	
	vector<cv::Vec4i> ContourFinder::getConvexityDefects(unsigned int i) const {
		return convexityDefects(contours[i]);
	}
	
	cv::RotatedRect ContourFinder::getMinAreaRect(unsigned int i) const {
		return minAreaRect(Mat(contours[i]));
	}
	
	cv::Point2f ContourFinder::getMinEnclosingCircle(unsigned int i, float& radius) const {
		cv::Point2f center;
		minEnclosingCircle(Mat(contours[i]), center, radius);
		return center;
	}
	
	cv::RotatedRect ContourFinder::getFitEllipse(unsigned int i) const {
		if(contours[i].size() < 5) {
			return getMinAreaRect(i);
		}
		return fitEllipse(Mat(contours[i]));
	}
	
	vector<cv::Point> ContourFinder::getFitQuad(unsigned int i) const {
		vector<cv::Point> convexHull = getConvexHull(i);		
		vector<cv::Point> quad = convexHull;
		
		static const unsigned int targetPoints = 4;
		static const unsigned int maxIterations = 16;
		static const double infinity = numeric_limits<double>::infinity();
		double minEpsilon = 0;
		double maxEpsilon = infinity;
		double curEpsilon = 16; // good initial guess
		
		// unbounded binary search to simplify the convex hull until it's 4 points
		if(quad.size() > 4) {
			for(int i = 0; i <(int) maxIterations; i++) {
				approxPolyDP(Mat(convexHull), quad, curEpsilon, true);
				if(quad.size() == targetPoints) {
					break;
				}
				if(quad.size() > targetPoints) {
					minEpsilon = curEpsilon;
					if(maxEpsilon == infinity) {
						curEpsilon = curEpsilon *  2;
					} else {
						curEpsilon = (maxEpsilon + minEpsilon) / 2;
					}
				}
				if(quad.size() < targetPoints) {
					maxEpsilon = curEpsilon;
					curEpsilon = (maxEpsilon + minEpsilon) / 2;
				}
			}
		}
		
		return quad;
	}
	
	cv::Vec2f ContourFinder::getVelocity(unsigned int i) const {
		return tracker.getVelocity(i);
	}
	
	unsigned int ContourFinder::getLabel(unsigned int i) const {
		return tracker.getCurrentLabels()[i];
	}
	
	RectTracker& ContourFinder::getTracker() {
		return tracker;
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
	
	void ContourFinder::setTargetColor(ofColor targetColor, TrackingColorMode trackingColorMode) {
		useTargetColor = true;
		this->targetColor = targetColor;
		this->trackingColorMode = trackingColorMode;
	}
	
	void ContourFinder::setSimplify(bool simplify) {
		this->simplify = simplify;
	}
	
	void ContourFinder::draw() {
		ofPushStyle();
		ofNoFill();
		for(int i = 0; i < (int)polylines.size(); i++) {
			polylines[i].draw();
			ofRect(toOf(getBoundingRect(i)));
		}
		ofPopStyle();
	}
	
	void ContourFinder::resetMinArea() {
		setMinArea(0);
	}
	
	void ContourFinder::resetMaxArea() {
		setMaxArea(numeric_limits<float>::infinity());
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
		this->minAreaNorm = true;
	}
	
	void ContourFinder::setMaxAreaNorm(float maxAreaNorm) {
		maxArea = maxAreaNorm;
		this->maxAreaNorm = true;
	}
	
}
