/* 
 this class is good for tracking things like faces. usually you want to speed
 up the tracking by doing detection on a smaller image, say setRescale(.25) for
 example to track on a 1/4 size image at a higher fps. other options for faster
 tracking include setCannyPruning(true) which will ignore low contrast regions,
 setFindBiggestObject(true) which finds the biggest object and returns it alone.
 it's rarely faster to do both of these together (learning opencv p 513).
 setMultiScaleFactor() provides a tradeoff between scale accuracy and speed.
 setMultiScaleFactor(1.01) will give a very accurate size for detected objects
 but will take longer to run. setMinNeighbors() tries to group multiple results
 from the tracker into a single result, and rejects anything that doesn't have
 enough results. setMinSizeScale() and setMaxSizeScale() set the minimum and
 maximum size for searching the space. you can set all these parameters at once
 with a preset using (for example) setPreset(ObjectFinder::Fast).
 
 need to add:
 - allow rotations
 */

#pragma once

#include "ofxCv/Utilities.h"
#include "ofxCv/Tracker.h"

#include "ofxCv.h"
namespace ofxCv {
	class ObjectFinder {
	public:
		
		ObjectFinder();
		void setup(string cascadeFilename);
		template <class T> 
		void update(T& img) {
			update(toCv(img));
		}
		void update(cv::Mat img);
		unsigned int size() const;
		ofRectangle getObject(unsigned int i) const;
		ofRectangle getObjectSmoothed(unsigned int i) const;
		RectTracker& getTracker();
		unsigned int getLabel(unsigned int i) const;
		cv::Vec2f getVelocity(unsigned int i) const;
		void draw() const;
		
		enum Preset {Fast,	Accurate, Sensitive};
		void setPreset(ObjectFinder::Preset preset);
		
		void setRescale(float rescale);
		void setMinNeighbors(int minNeighbors);
		void setMultiScaleFactor(float multiScaleFactor);
		void setCannyPruning(bool cannyPruning);
		void setFindBiggestObject(bool findBiggestObject);
		void setUseHistogramEqualization(bool useHistogramEqualization);
		void setMinSizeScale(float minSizeScale);
		void setMaxSizeScale(float maxSizeScale);
		
		float getRescale() const;
		int getMinNeighbors() const;
		float getMultiScaleFactor() const;
		bool getCannyPruning() const;
		bool getFindBiggestObject() const;
		bool getUseHistogramEqualization() const;
		float getMinSizeScale() const;
		float getMaxSizeScale() const;
		
	protected:
		float rescale, multiScaleFactor;
		int minNeighbors;
		bool useHistogramEqualization, cannyPruning, findBiggestObject;
		float minSizeScale, maxSizeScale;
		cv::Mat gray, graySmall;
		cv::CascadeClassifier classifier;
		vector<cv::Rect> objects;
		RectTracker tracker;
	};
}
