#pragma once

// OpenCV 2
#include "cv.h"
#include "highgui.h"

// ofxCv
#include "FloatImage.h"
#include "Distance.h"
#include "Calibration.h"

// openFrameworks
#include "ofConstants.h"
#include "ofImage.h"

/*
 all functions guarantee internally that the output images imitates input
 there should be a ton more const functions, but it's hard because of OF const issues
 need to print better errors than default opencv errors...?
*/

namespace ofxCv {
	
	using namespace cv;
	
	// imitate() is good for preparing buffers
	// it's like allocate(), but uses the size and type of the original as a reference
	void imitate(ofPixels& mirror, ofPixels& original);
	void imitate(ofImage& mirror, ofPixels& original);
	void imitate(FloatImage& mirror, ofPixels& original);
	void imitate(FloatImage& mirror, FloatImage& original);
	
	template <class T>
	void imitate(T& mirror, ofBaseHasPixels& original) {
		imitate(mirror, original.getPixelsRef());
	}
	
	void copy(Mat to, Mat from);
	
	// conversion functions toOf() and toCv()
	Mat toCv(ofPixels& pix);
	Mat toCv(ofBaseHasPixels& img);
	Mat toCv(FloatImage& img);
	Mat toCv(ofMesh& mesh);
	Point3f toCv(ofVec3f& vec);
	cv::Rect toCv(const ofRectangle& rect);
	int toCv(const ofImageType& ofType);
	
	ofVec2f toOf(Point2f point);
	ofVec3f toOf(Point3f point);
	ofRectangle toOf(cv::Rect& rect);
	
	void loadImage(Mat& mat, string filename);
	void saveImage(Mat& mat, string filename);
	
	ofMatrix4x4 makeMatrix(Mat rotation, Mat translation);
	
	void drawMat(Mat& mat, float x, float y);
	void drawMat(Mat& mat, float x, float y, float width, float height);
	
	void applyMatrix(const ofMatrix4x4& matrix);
	
	void invert(ofImage& img);
	
	void rotate(ofImage& source, ofImage& destination, double angle, unsigned char fill = 0, int interpolation = INTER_LINEAR);
	void autorotate(ofImage& original, ofImage& thresh, ofImage& output, float* rotation = NULL);
	void autothreshold(ofImage& original, ofImage& thresh, bool invert = false);
	void autothreshold(ofImage& original, bool invert = false);
	void threshold(FloatImage& img, float value, bool invert = false);
	void threshold(FloatImage& original, FloatImage& thresh, float value, bool invert = false);
	void matchRegion(ofImage& source, ofRectangle& region, ofImage& search, FloatImage& result);
	void matchRegion(Mat& source, ofRectangle& region, Mat& search, Mat& result);
	void convolve(ofImage& source, FloatImage& kernel, ofImage& destination);
	void convolve(ofImage& img, FloatImage& kernel);
	void blur(FloatImage& original, FloatImage& blurred, int size);
	void medianBlur(ofImage& img, int size);
	void warpPerspective(ofImage& src, ofImage& dst, Mat& m, int flags = 0);
	void warpPerspective(ofPixels& src, ofPixels& dst, Mat& m, int flags = 0);
	
	// options: INTER_NEAREST, INTER_LINEAR, INTER_AREA, INTER_CUBIC, INTER LANCZOS4
	void resize(ofImage& source, ofImage& destination, int interpolation = INTER_LINEAR);
	void resize(ofImage& source, ofImage& destination, float xScale, float yScale, int interpolation = INTER_LINEAR);
	
	ofVec2f findMaxLocation(FloatImage& img);
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
	
	float weightedAverageAngle(const vector<Vec4i>& lines);
	
}

// <3 kyle
