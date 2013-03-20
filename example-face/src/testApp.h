#pragma once

#include "ofMain.h"
#include "ofxCv.h"

/*
 A classifier helper should handle the initial resizing for you. It should
 have two "preset" modes for very accurate face finding and very fast.
 It should also do automatic drawing, and give you OF objects instead of CV
 objects. It should also allow rotations, and handle tracking.
 */

namespace ofxCv {
	class ObjectFinder {
	protected:
		float rescale;
		bool useHistogramEqualization;
		cv::Mat gray, graySmall;
		cv::CascadeClassifier classifier;
		vector<cv::Rect> objects;
		
	public:
		ObjectFinder()
		:rescale(1)
		,useHistogramEqualization(true)
		{
		}
		void setup(string cascadeFilename) {
			classifier.load(ofToDataPath(cascadeFilename));
		}		
		template <class T> 
		void update(T& img) {
			update(toCv(img));
		}
		void update(cv::Mat img) {
			convertColor(img, gray, CV_RGB2GRAY);
			resize(gray, graySmall);
			cv::Mat graySmallMat = toCv(graySmall);
			if(useHistogramEqualization) {
				equalizeHist(graySmallMat, graySmallMat);
			}
			classifier.detectMultiScale(graySmallMat, objects, 1.06, 1,
																	//CascadeClassifier::DO_CANNY_PRUNING |
																	//CascadeClassifier::FIND_BIGGEST_OBJECT |
																	//CascadeClassifier::DO_ROUGH_SEARCH |
																	0);
		}		
		unsigned int size() const {
			return objects.size();
		}
		ofRectangle getObject(int i) const {
			ofRectangle rect = toOf(objects[i]);
			rect.scale(1 / rescale);
			return rect;
		}
		void setRescale() {
			this->rescale = rescale;
		}
		float getRescale() const {
			return rescale;
		}
		void setUseHistogramEqualization(bool useHistogramEqualization) {
			this->useHistogramEqualization = useHistogramEqualization;
		}
		bool getUseHistogramEqualization() const {
			return useHistogramEqualization;
		}
	};
}

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	ofVideoGrabber cam;
	ofxCv::ObjectFinder finder;
};
