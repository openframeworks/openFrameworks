#include "ObjectFinder.h"

namespace ofxCv {
	ObjectFinder::ObjectFinder()
	:rescale(1)
	,multiScaleFactor(1.1)
	,minNeighbors(3)
	,minSizeScale(0)
	,maxSizeScale(1)
	,useHistogramEqualization(true)
	,cannyPruning(false)
	,findBiggestObject(false)
	{
	}
	void ObjectFinder::setup(string cascadeFilename) {
		cascadeFilename = ofToDataPath(cascadeFilename);
		if(ofFile(cascadeFilename).exists()) {
			classifier.load(cascadeFilename);
		} else {
			ofLogError("ObjectFinder::setup") << "Couldn't find " << cascadeFilename;
		}
	}
	void ObjectFinder::update(cv::Mat img) {
		convertColor(img, gray, CV_RGB2GRAY);
		resize(gray, graySmall, rescale, rescale);
		cv::Mat graySmallMat = toCv(graySmall);
		if(useHistogramEqualization) {
			equalizeHist(graySmallMat, graySmallMat);
		}
		cv::Size minSize, maxSize;
		float averageSide = (graySmallMat.rows + graySmallMat.cols) / 2;
		if(minSizeScale > 0) {
			int side = minSizeScale * averageSide;
			minSize = cv::Size(side, side);
		}
		if(maxSizeScale < 1) {
			int side = maxSizeScale * averageSide;
			maxSize = cv::Size(side, side);
		}
		classifier.detectMultiScale(graySmallMat, objects, multiScaleFactor, minNeighbors,
																(cannyPruning ? CASCADE_DO_CANNY_PRUNING : 0) |
																(findBiggestObject ? CASCADE_FIND_BIGGEST_OBJECT | CASCADE_DO_ROUGH_SEARCH : 0),
																minSize,
																maxSize);
		for(int i = 0; i < objects.size(); i++) {
			cv::Rect& rect = objects[i];
			rect.width /= rescale, rect.height /= rescale;
			rect.x /= rescale, rect.y /= rescale;
		}
		tracker.track(objects);
	}
	unsigned int ObjectFinder::size() const {
		return objects.size();
	}
	ofRectangle ObjectFinder::getObject(unsigned int i) const {
		return toOf(objects[i]);
	}
	ofRectangle ObjectFinder::getObjectSmoothed(unsigned int i) const {
		return toOf(tracker.getSmoothed(getLabel(i)));
	}
	cv::Vec2f ObjectFinder::getVelocity(unsigned int i) const {
		return tracker.getVelocity(i);
	}
	unsigned int ObjectFinder::getLabel(unsigned int i) const {
		return tracker.getCurrentLabels()[i];
	}
	RectTracker& ObjectFinder::getTracker() {
		return tracker;
	}
	void ObjectFinder::draw() const {
		ofPushStyle();
		ofNoFill();
		for(int i = 0; i < size(); i++) {
			ofRectangle object = getObject(i);
			ofRect(object);
			ofDrawBitmapStringHighlight(ofToString(getLabel(i)), object.x, object.y);
		}
		ofPopStyle();
	}
	void ObjectFinder::setPreset(ObjectFinder::Preset preset) {
		if(preset == ObjectFinder::Fast) {
			setRescale(.25);
			setMinNeighbors(2);
			setMultiScaleFactor(1.2);
			setMinSizeScale(.25);
			setMaxSizeScale(.75);
			setCannyPruning(true);
			setFindBiggestObject(false);
		} else if(preset == ObjectFinder::Accurate) {
			setRescale(.5);
			setMinNeighbors(6);
			setMultiScaleFactor(1.02);
			setMinSizeScale(.1);
			setMaxSizeScale(1);
			setCannyPruning(true);
			setFindBiggestObject(false);			
		} else if(preset == ObjectFinder::Sensitive) {
			setRescale(.5);
			setMinNeighbors(1);
			setMultiScaleFactor(1.02);
			setMinSizeScale(.1);
			setMaxSizeScale(1);
			setCannyPruning(false);
			setFindBiggestObject(false);
		}
	}
	
	void ObjectFinder::setRescale(float rescale) {
		this->rescale = rescale;
	}
	void ObjectFinder::setMinNeighbors(int minNeighbors) {
		this->minNeighbors = minNeighbors;
	}
	void ObjectFinder::setMultiScaleFactor(float multiScaleFactor) {
		this->multiScaleFactor = multiScaleFactor;
	}
	void ObjectFinder::setCannyPruning(bool cannyPruning) {
		this->cannyPruning = cannyPruning;
	}
	void ObjectFinder::setFindBiggestObject(bool findBiggestObject) {
		this->findBiggestObject = findBiggestObject;
	}
	void ObjectFinder::setUseHistogramEqualization(bool useHistogramEqualization) {
		this->useHistogramEqualization = useHistogramEqualization;
	}
	void ObjectFinder::setMinSizeScale(float minSizeScale) {
		this->minSizeScale = minSizeScale;
	}
	void ObjectFinder::setMaxSizeScale(float maxSizeScale) {
		this->maxSizeScale = maxSizeScale;
	}
	
	float ObjectFinder::getRescale() const {
		return rescale;
	}
	int ObjectFinder::getMinNeighbors() const {
		return minNeighbors;
	}
	float ObjectFinder::getMultiScaleFactor() const {
		return multiScaleFactor;
	}
	bool ObjectFinder::getCannyPruning() const {
		return cannyPruning;
	}
	bool ObjectFinder::getFindBiggestObject() const {
		return findBiggestObject;
	}
	bool ObjectFinder::getUseHistogramEqualization() const {
		return useHistogramEqualization;
	}
	float ObjectFinder::getMinSizeScale() const {
		return minSizeScale;
	}
	float ObjectFinder::getMaxSizeScale() const {
		return maxSizeScale;
	}
}
