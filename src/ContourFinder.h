#pragma once

#include "ofxCv.h"

/*
 the contour finder will automatically threshold your image for you. it will
 also convert non-grayscale images to grayscale before thresholding. if you
 want to disable this, setAutoThreshold(false). by default the threshold is
 128. to change this, use setThreshold().
 
 by default, the results are unfiltered by area. to filter by area use one of
 set(Min/Max)(Area/Radius/Norm) functions. set(Min/Max)Area is in pixels.
 set(Min/Max)Radius uses the area of a circle with the given radius for a more
 "linear" feeling. set(Min/Max)Norm uses values between (0-1) and multiplies
 by the input image area. to reset the min/max area call reset(Min/Max)Area.
 
 keeping with the ofxCv philosophy, no new objects (a la ofxCvBlob) are used.
 you can get contours as vector<cv::Point> or ofPolyline. for other features,
 you can use methods of ofPolyline (getArea(), getPerimiter()) or cv methods
 by asking ContourFinder (getContourArea(), getArcLength()).
 */

// to implement in ContourFinder:
// color tracking (RGB/HSV)
// holes/no holes
// cv::pointPolygonTest - inside, edge, outside
// cv::matchShapes - similarity between two contours
// cv::estimateRigidTransform? subdivision-based estimation for outline-flow?
// CV_THRESH_OTSU?

namespace ofxCv {

	class ContourFinder {
	public:
		ContourFinder();
		
		template <class T> 
		void findContours(T& img) {
			findContours(toCv(img));
		}
		void findContours(cv::Mat img);
		vector<vector<cv::Point> >& getContours();
		vector<ofPolyline>& getPolylines();
		
		unsigned int size() const;
		vector<cv::Point>& getContour(unsigned int i);
		ofPolyline& getPolyline(unsigned int i);
		
		cv::Rect getBoundingRect(unsigned int i) const;
		cv::Point2f getCenter(unsigned int i) const; // center of bounding box (most stable)
		cv::Point2f getCentroid(unsigned int i) const; // center of mass (less stable)
		cv::Point2f getAverage(unsigned int i) const; // average of contour vertices (least stable)
		cv::Vec2f getBalance(unsigned int i) const; // difference between centroid and center
		double getContourArea(unsigned int i) const;
		double getArcLength(unsigned int i) const;
		vector<cv::Point> getConvexHull(unsigned int i) const;
		cv::RotatedRect getMinAreaRect(unsigned int i) const;
		cv::RotatedRect getFitEllipse(unsigned int i) const;
		
		void setAutoThreshold(bool autoThreshold);
		void setThreshold(float thresholdValue);
		void setInvert(bool invert);
		
		// should accept an ofColor or a cv::Scalar?
		// TRACK_RGB, TRACK_HSV, TRACK_HUE
		// use inRange() to do the thresholding (no need for distance calculation)
		void setTargetColor(ofColor targetColor, bool useHsv = false);
		
		void resetMinArea();
		void resetMaxArea();
		void setMinArea(float minArea);
		void setMaxArea(float maxArea);
		void setMinAreaRadius(float minAreaRadius);
		void setMaxAreaRadius(float maxAreaRadius);
		void setMinAreaNorm(float minAreaNorm);
		void setMaxAreaNorm(float maxAreaNorm);
		
		void setSimplify(bool simplify);
		
		void draw();
	protected:
		cv::Mat thresh;
		bool autoThreshold, invert, simplify;
		float thresholdValue;
		
		bool useTargetColor;
		bool useHsv;
		ofColor targetColor;
		
		float minArea, maxArea;
		bool minAreaNorm, maxAreaNorm;
		
		vector<vector<cv::Point> > contours;
		vector<ofPolyline> polylines;
	};	
	
}