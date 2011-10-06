/*
 helpers offer new, commonly-needed functionality that is not present in OpenCv
 or openFrameworks.
*/

#pragma once

#include "opencv2/opencv.hpp"
#include "ofMain.h"

namespace ofxCv {

	using namespace cv;

	void loadImage(Mat& mat, string filename);
	void saveImage(Mat mat, string filename);
	void loadMat(Mat& mat, string filename);
	void saveMat(Mat mat, string filename);
	
	ofMatrix4x4 makeMatrix(Mat rotation, Mat translation);
	void applyMatrix(const ofMatrix4x4& matrix);
	
	void drawMat(Mat& mat, float x, float y);
	void drawMat(Mat& mat, float x, float y, float width, float height);
	
	//ofVec2f findMaxLocation(FloatImage& img);
	ofVec2f findMaxLocation(Mat& mat);
	
	void getBoundingBox(ofImage& img, ofRectangle& box, int thresh = 1, bool invert = false);
	int findFirst(const Mat& arr, unsigned char target);
	int findLast(const Mat& arr, unsigned char target);
	
	// sometimes you want a different datatype returned than the one you pass in
	// but the same number of channels. with mean and sum, you *probably* want floating point.
	Mat meanCols(const Mat& mat);
	Mat meanRows(const Mat& mat);
	Mat sumCols(const Mat& mat);
	Mat sumRows(const Mat& mat);
	
	Mat minCols(const Mat& mat);
	Mat minRows(const Mat& mat);
	Mat maxCols(const Mat& mat);
	Mat maxRows(const Mat& mat);
	
	float weightedAverageAngle(const vector<Vec4i>& lines);
	
	// (nearest point) to the two given lines
	template <class T>
	Point3_<T> intersectLineLine(Point3_<T> lineStart1, Point3_<T> lineEnd1, Point3_<T> lineStart2, Point3_<T> lineEnd2) {
		Point3_<T> v1(lineEnd1 - lineStart1), v2(lineEnd2 - lineStart2);
		T v1v1 = v1.dot(v1), v2v2 = v2.dot(v2), v1v2 = v1.dot(v2), v2v1 = v2.dot(v1);
		Mat_<T> lambda = (1. / (v1v1 * v2v2 - v1v2 * v1v2))
			* ((Mat_<T>(2, 2) << v2v2, v1v2, v2v1, v1v1)
			 * (Mat_<T>(2, 1) << v1.dot(lineStart2 - lineStart1), v2.dot(lineStart1 - lineStart2)));
		return (1./2) * ((lineStart1 + v1 * lambda(0)) + (lineStart2 + v2 * lambda(1)));
	}

	// (nearest point on a line) to the given point
	template <class T>
	Point3_<T> intersectPointLine(Point3_<T> point, Point3_<T> lineStart, Point3_<T> lineEnd) {
		Point3_<T> ray = lineEnd - lineStart;
		T u = (point - lineStart).dot(ray) / ray.dot(ray);
		return lineStart + u * ray;
	}

	// (nearest point on a ray) to the given point
	template <class T>
	Point3_<T> intersectPointRay(Point3_<T> point, Point3_<T> ray) {
		return ray * (point.dot(ray) / ray.dot(ray));
	}
	
	static const ofColor cyanPrint = ofColor::fromHex(0x00abec);
	static const ofColor magentaPrint = ofColor::fromHex(0xec008c);
	static const ofColor yellowPrint = ofColor::fromHex(0xffee00);
	
	void drawHighlightString(string text, ofPoint position, ofColor background = ofColor::black, ofColor foreground = ofColor::white);
	void drawHighlightString(string text, int x, int y, ofColor background = ofColor::black, ofColor foreground = ofColor::white);
	
}