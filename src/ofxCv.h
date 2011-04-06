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
 some things for ofxCv to solve:
 1 wrap complex things in an easy way: easier functions, and provide demos
 2 buffer management (is this still necessary in cv2?)
 3 make it more obvious what's available (e.g., member methods)
 4 lightweight CV <-> OF datatype conversion
 
 best thing would be being able to use opencv but with ofImages...
 
 e.g.:
 ofImage img;
 img.loadImage("img.png");
 cv::bitwise_not(img, img);
 // but, more likely
 cv::bitwise_not(ofMat(img), ofMat(img));
 // and: class ofMat : public Mat {ofMat(ofImage& img);};
 
 all functions guarantee internally that the output images imitates input
 there should be a ton more const functions, but it's hard because of OF const issues
 need to print better errors than default opencv errors...?
 if ofMat has a buffer image in it, then every function that needs a buffer can use that
 how is cv2 dealing w buffers?
 */

namespace ofxCv {
	
	using namespace cv;
	
	// util functions
	void imitate(ofImage& mirror, ofImage& original);
	void imitate(FloatImage& mirror, ofImage& original);
	void imitate(FloatImage& mirror, FloatImage& original);
	int getCvImageType(const ofImageType& ofType);
	Mat getMat(ofImage& img);
	ofVec2f makeVec(Point2f point);
	ofVec3f makeVec(Point3f point);
	cv::Rect makeRect(ofRectangle& rect);
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