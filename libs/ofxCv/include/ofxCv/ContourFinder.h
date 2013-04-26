/* 
 the contour finder will automatically convert and threshold your image for you.
 by default, it finds bright regions. to find dark regions call setInvert(true).
 to track a color, call setTargetColor(). by default, it tracks in RGB space.
 to track in HSV or just hue space, pass TRACK_COLOR_HSV or TRACK_COLOR_H.
 to change the threshold value, use setThreshold(). by default, the threshold is
 128. when finding bright regions, 128 is halfway between white and black. when
 tracking a color, 0 means "exactly similar" and 255 is "all colors".
 
 by default, the results are unfiltered by area. to filter by area use one of
 set(Min/Max)(Area/Radius/Norm) functions. set(Min/Max)Area is in pixels.
 set(Min/Max)Radius uses the area of a circle with the given radius for a more
 "linear" feeling. set(Min/Max)Norm uses values between (0-1) and multiplies
 by the input image area. to reset the min/max area call reset(Min/Max)Area.
 
 keeping with the ofxCv philosophy, no new objects (like ofxCvBlob) are used.
 you can get contours as vector<cv::Point> or ofPolyline. for other features,
 you can use methods of ofPolyline (getArea(), getPerimiter()) or cv methods
 by asking ContourFinder (getContourArea(), getArcLength()).
 */

// to implement in ContourFinder:
// holes/no holes
// CV_THRESH_OTSU?
// cv::pointPolygonTest - inside, edge, outside
// cv::matchShapes - similarity between two contours
// cv::estimateRigidTransform? subdivision-based estimation for outline-flow?

#pragma once

#include "ofxCv/Utilities.h"
#include "ofxCv/Tracker.h"

namespace ofxCv {
	
	enum TrackingColorMode {TRACK_COLOR_RGB, TRACK_COLOR_HSV, TRACK_COLOR_H, TRACK_COLOR_HS};
	
	class ContourFinder {
	public:
		ContourFinder();
		
		template <class T> 
		void findContours(T& img) {
			findContours(toCv(img));
		}
		void findContours(cv::Mat img);
		const vector<vector<cv::Point> >& getContours() const;
		const vector<ofPolyline>& getPolylines() const;
		const vector<cv::Rect>& getBoundingRects() const;
		
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
		vector<cv::Vec4i> getConvexityDefects(unsigned int i) const;
		cv::RotatedRect getMinAreaRect(unsigned int i) const;
		cv::Point2f getMinEnclosingCircle(unsigned int i, float& radius) const;
		cv::RotatedRect getFitEllipse(unsigned int i) const;
		vector<cv::Point> getFitQuad(unsigned int i) const;
		cv::Vec2f getVelocity(unsigned int i) const;
		
		RectTracker& getTracker();
		unsigned int getLabel(unsigned int i) const;
		
		void setThreshold(float thresholdValue);
		void setAutoThreshold(bool autoThreshold);
		void setInvert(bool invert);
		void setTargetColor(ofColor targetColor, TrackingColorMode trackingColorMode = TRACK_COLOR_RGB);
		void setFindHoles(bool findHoles);
		
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
		cv::Mat hsvBuffer, thresh;
		bool autoThreshold, invert, simplify;
		float thresholdValue;
		
		bool useTargetColor;
		TrackingColorMode trackingColorMode;
		ofColor targetColor;
		
		float minArea, maxArea;
		bool minAreaNorm, maxAreaNorm;
		
		vector<vector<cv::Point> > contours;
		vector<ofPolyline> polylines;
		
		RectTracker tracker;
		vector<cv::Rect> boundingRects;

		int contourFindingMode;
	};	
	
}
