/*
 helpers offer new, commonly-needed functionality that is not quite present in
 OpenCv or openFrameworks.
 */

#pragma once

#include "opencv2/opencv.hpp"
#include "ofVectorMath.h"
#include "ofRectangle.h"
#include "ofColor.h"

namespace ofxCv {
	
	ofMatrix4x4 makeMatrix(cv::Mat rotation, cv::Mat translation);
	void applyMatrix(const ofMatrix4x4& matrix);
	
	void drawMat(cv::Mat& mat, float x, float y);
	void drawMat(cv::Mat& mat, float x, float y, float width, float height);
	
	template <class T>
	ofVec2f findMaxLocation(T& img) {
		cv::Mat mat = toCv(img);
		double minVal, maxVal;
		cv::Point minLoc, maxLoc;
		minMaxLoc(mat, &minVal, &maxVal, &minLoc, &maxLoc);
		return ofVec2f(maxLoc.x, maxLoc.y);
	}
	
	template <class T>
	cv::Mat meanCols(T& img) {
		cv::Mat mat = toCv(img);
		cv::Mat colMat(mat.cols, 1, mat.type());
		for(int i = 0; i < mat.cols; i++) {
			colMat.row(i) = mean(mat.col(i));
		}	
		return colMat;
	}
	
	template <class T>
	cv::Mat meanRows(T& img) {
		cv::Mat mat = toCv(img);
		cv::Mat rowMat(mat.rows, 1, mat.type());
		for(int i = 0; i < mat.rows; i++) {
			rowMat.row(i) = mean(mat.row(i));
		}
		return rowMat;
	}
	
	template <class T>
	cv::Mat sumCols(T& img) {
		cv::Mat mat = toCv(img);
		cv::Mat colMat(mat.cols, 1, CV_32FC1);
		for(int i = 0; i < mat.cols; i++) {
			colMat.row(i) = sum(mat.col(i));
		}	
		return colMat;
	}
	
	template <class T>
	cv::Mat sumRows(T& img) {
		cv::Mat mat = toCv(img);
		cv::Mat rowMat(mat.rows, 1, CV_32FC1);
		for(int i = 0; i < mat.rows; i++) {
			rowMat.row(i) = sum(mat.row(i));
		}
		return rowMat;
	}
	
	template <class T>
	cv::Mat minCols(T& img) {
		cv::Mat mat = toCv(img);
		cv::Mat colMat(mat.cols, 1, CV_32FC1);
		double minVal, maxVal;
		for(int i = 0; i < mat.cols; i++) {
			minMaxLoc(mat.col(i), &minVal, &maxVal); 
			colMat.row(i) = minVal;
		}	
		return colMat;
	}
	
	template <class T>
	cv::Mat minRows(T& img) {
		cv::Mat mat = toCv(img);
		cv::Mat rowMat(mat.rows, 1, CV_32FC1);
		double minVal, maxVal;
		for(int i = 0; i < mat.rows; i++) {
			minMaxLoc(mat.row(i), &minVal, &maxVal); 
			rowMat.row(i) = minVal;
		}
		return rowMat;
	}
	
	template <class T>
	cv::Mat maxCols(T& img) {
		cv::Mat mat = toCv(img);
		cv::Mat colMat(mat.cols, 1, CV_32FC1);
		double minVal, maxVal;
		for(int i = 0; i < mat.cols; i++) {
			minMaxLoc(mat.col(i), &minVal, &maxVal); 
			colMat.row(i) = maxVal;
		}	
		return colMat;
	}
	
	template <class T>
	cv::Mat maxRows(T& img) {
		cv::Mat mat = toCv(img);
		cv::Mat rowMat(mat.rows, 1, CV_32FC1);
		double minVal, maxVal;
		for(int i = 0; i < mat.rows; i++) {
			minMaxLoc(mat.row(i), &minVal, &maxVal); 
			rowMat.row(i) = maxVal;
		}
		return rowMat;
	}
	
	int findFirst(const cv::Mat& arr, unsigned char target);
	int findLast(const cv::Mat& arr, unsigned char target);
	
	template <class T>
	void getBoundingBox(T& img, ofRectangle& box, int thresh, bool invert) {
		cv::Mat mat = toCv(img);
		int flags = (invert ? cv::THRESH_BINARY_INV : cv::THRESH_BINARY);
		
		cv::Mat rowMat = meanRows(mat);
		threshold(rowMat, rowMat, thresh, 255, flags);
		box.y = findFirst(rowMat, 255);
		box.height = findLast(rowMat, 255);
		box.height -= box.y;
		
		cv::Mat colMat = meanCols(mat);
		threshold(colMat, colMat, thresh, 255, flags);
		box.x = findFirst(colMat, 255);
		box.width = findLast(colMat, 255);
		box.width -= box.x;
	}
	
	float weightedAverageAngle(const vector<cv::Vec4i>& lines);
	
	// (nearest point) to the two given lines
	template <class T>
	cv::Point3_<T> intersectLineLine(cv::Point3_<T> lineStart1, cv::Point3_<T> lineEnd1, cv::Point3_<T> lineStart2, cv::Point3_<T> lineEnd2) {
		cv::Point3_<T> v1(lineEnd1 - lineStart1), v2(lineEnd2 - lineStart2);
		T v1v1 = v1.dot(v1), v2v2 = v2.dot(v2), v1v2 = v1.dot(v2), v2v1 = v2.dot(v1);
		cv::Mat_<T> lambda = (1. / (v1v1 * v2v2 - v1v2 * v1v2))
		* ((cv::Mat_<T>(2, 2) << v2v2, v1v2, v2v1, v1v1)
			 * (cv::Mat_<T>(2, 1) << v1.dot(lineStart2 - lineStart1), v2.dot(lineStart1 - lineStart2)));
		return (1./2) * ((lineStart1 + v1 * lambda(0)) + (lineStart2 + v2 * lambda(1)));
	}
	
	// (nearest point on a line) to the given point
	template <class T>
	cv::Point3_<T> intersectPointLine(cv::Point3_<T> point, cv::Point3_<T> lineStart, cv::Point3_<T> lineEnd) {
		cv::Point3_<T> ray = lineEnd - lineStart;
		T u = (point - lineStart).dot(ray) / ray.dot(ray);
		return lineStart + u * ray;
	}
	
	// (nearest point on a ray) to the given point
	template <class T>
	cv::Point3_<T> intersectPointRay(cv::Point3_<T> point, cv::Point3_<T> ray) {
		return ray * (point.dot(ray) / ray.dot(ray));
	}
	
	// morphological thinning, also called skeletonization, strangely missing from opencv
	// here is a description of the algorithm http://homepages.inf.ed.ac.uk/rbf/HIPR2/thin.htm
	template <class T>
	void thin(T& img) {
		cv::Mat mat = toCv(img);
		int w = mat.cols, h = mat.rows;
		int ia1=-w-1,ia2=-w-0,ia3=-w+1,ib1=-0-1,ib3=-0+1,ic1=+w-1,ic2=+w-0,ic3=+w+1;
		unsigned char* p = mat.ptr<unsigned char>();
		vector<unsigned int> q;
		for(int y = 1; y + 1 < h; y++) {
			for(int x = 1; x + 1 < w; x++) {
				int i = y * w + x;
				if(p[i]) {
					q.push_back(i);
				}
			}
		}
		int n = q.size();	
		for(int i=0;i<n;i++){int j=q[i];if(!p[j+ia1]&&!p[j+ia2]&&!p[j+ia3]&&p[j+ic1]&&p[j+ic2]&&p[j+ic3]){p[j]=0;}}
		for(int i=0;i<n;i++){int j=q[i];if(!p[j+ia3]&&!p[j+ib3]&&!p[j+ic3]&&p[j+ia1]&&p[j+ib1]&&p[j+ic1]){p[j]=0;}}
		for(int i=0;i<n;i++){int j=q[i];if(!p[j+ic1]&&!p[j+ic2]&&!p[j+ic3]&&p[j+ia1]&&p[j+ia2]&&p[j+ia3]){p[j]=0;}}
		for(int i=0;i<n;i++){int j=q[i];if(!p[j+ia1]&&!p[j+ib1]&&!p[j+ic1]&&p[j+ia3]&&p[j+ib3]&&p[j+ic3]){p[j]=0;}}
		for(int i=0;i<n;i++){int j=q[i];if(!p[j+ia2]&&!p[j+ia3]&&!p[j+ib3]&&p[j+ib1]&&p[j+ic2]){p[j]=0;}}
		for(int i=0;i<n;i++){int j=q[i];if(!p[j+ib3]&&!p[j+ic3]&&!p[j+ic2]&&p[j+ib1]&&p[j+ia2]){p[j]=0;}}
		for(int i=0;i<n;i++){int j=q[i];if(!p[j+ic2]&&!p[j+ic1]&&!p[j+ib1]&&p[j+ia2]&&p[j+ib3]){p[j]=0;}}
		for(int i=0;i<n;i++){int j=q[i];if(!p[j+ib1]&&!p[j+ia1]&&!p[j+ia2]&&p[j+ic2]&&p[j+ib3]){p[j]=0;}}
	}
	
	// given a vector of lines, this function will find the average angle
	float weightedAverageAngle(const vector<cv::Vec4i>& lines);
	
	// finds the average angle of hough lines, unrotates by that amount and
	// returns the average rotation. you can supply your own thresholded image
	// for hough lines, or let it run canny detection for you.
	template <class S, class T, class D>
	float autorotate(S& src, D& dst, float threshold1 = 50, float threshold2 = 200) {
		cv::Mat thresh;
		cv::Canny(src, thresh, threshold1, threshold2);
		return autorotate(src, thresh, dst);
	}
	
	template <class S, class T, class D>
	float autorotate(S& src, T& thresh, D& dst) {
		imitate(dst, src);
		cv::Mat srcMat = toCv(src), threshMat = toCv(thresh);
		std::vector<cv::Vec4i> lines;
		double distanceResolution = 1;
		double angleResolution = CV_PI / 180;
		// these three values are just heuristics that have worked for me
		int voteThreshold = 10;
		double minLineLength = (srcMat.rows + srcMat.cols) / 8;
		double maxLineGap = 3;
		HoughLinesP(threshMat, lines, distanceResolution, angleResolution, voteThreshold, minLineLength, maxLineGap);
		float rotationAmount = ofRadToDeg(weightedAverageAngle(lines));
		rotate(src, dst, rotationAmount);
		return rotationAmount;
	}
	
	std::vector<cv::Point2f> getConvexPolygon(const std::vector<cv::Point2f>& convexHull, int targetPoints);
	
	static const ofColor cyanPrint = ofColor::fromHex(0x00abec);
	static const ofColor magentaPrint = ofColor::fromHex(0xec008c);
	static const ofColor yellowPrint = ofColor::fromHex(0xffee00);
	
	void drawHighlightString(string text, ofPoint position, ofColor background = ofColor::black, ofColor foreground = ofColor::white);
	void drawHighlightString(string text, int x, int y, ofColor background = ofColor::black, ofColor foreground = ofColor::white);
}
