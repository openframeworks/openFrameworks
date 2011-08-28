#pragma once

#include "opencv2/opencv.hpp"
#include "ofMain.h"
#include "stdint.h"

// ofxCv
#include "Distance.h"
#include "Calibration.h"
#include "ContourFinder.h"

/*
 there should be a few kinds of functions:
 1 utility functions like imitate and toCv
 2 wrapper functions that accept toCv-compatible objects
 
 all functions should guarantee the size of the output with imitate when necessary
 all output should be done using function arguments, to keep things flexible
 
 there should be a ton more const functions, but it's hard because of OF const issues
 need to print better errors than default opencv errors...?
 
 if it's possible to allocate an unallocated image, ofxCv should allocate it.
 */

namespace ofxCv {
	
	using namespace cv;
	
	// 1 utility functions
	
	// toCv functions
	Mat toCv(Mat& mat);
	template <class T> inline Mat toCv(ofPixels_<T>& pix) {
		return Mat(pix.getHeight(), pix.getWidth(), getCvImageType(pix), pix.getPixels(), 0);
	}
	template <class T> inline Mat toCv(ofImage_<T>& img) {
		return Mat(img.getHeight(), img.getWidth(), getCvImageType(img), img.getPixels(), 0);
	}
	Mat toCv(ofBaseHasPixels& img);
	Mat toCv(ofMesh& mesh);
	Point2f toCv(ofVec2f& vec);
	Point3f toCv(ofVec3f& vec);
	cv::Rect toCv(const ofRectangle& rect);
	vector<cv::Point2f> toCv(const ofPolyline& polyline);
		
	// toOf functions
	ofVec2f toOf(Point2f point);
	ofVec3f toOf(Point3f point);
	ofRectangle toOf(cv::Rect rect);
	ofPolyline toOf(cv::RotatedRect& rect);
	template <class T> inline ofPolyline toOf(const vector<cv::Point_<T> >& contour) {
		ofPolyline polyline;
		polyline.resize(contour.size());
		for(int i = 0; i < contour.size(); i++) {
			polyline[i].x = contour[i].x;
			polyline[i].y = contour[i].y;
		}
		polyline.close();
		return polyline;
	}
	
	// these functions are for accessing Mat, ofPixels and ofImage consistently.
	// they're very important for imitate().
	
	// width, height
	template <class T> inline int getWidth(T& src) {return src.getWidth();}
	template <class T> inline int getHeight(T& src) {return src.getHeight();}
	inline int getWidth(Mat& src) {return src.cols;}
	inline int getHeight(Mat& src) {return src.rows;}
	
	// depth
	inline int getDepth(Mat& mat) {
		return mat.depth();
	}
	template <class T> inline int getDepth(ofPixels_<T>& pixels) {
		switch(pixels.getBytesPerChannel()) {
			case 3: return CV_32F;
			case 2: return CV_16U;
			case 1: default: return CV_8U;
		}
	}
	template <class T> inline int getDepth(ofBaseHasPixels_<T>& img) {
		return getDepth(img.getPixelsRef());
	}
	
	// channels
	inline int getChannels(int cvImageType) {
		return CV_MAT_CN(cvImageType);
	}
	inline int getChannels(ofImageType imageType) {
		switch(imageType) {
			case OF_IMAGE_COLOR_ALPHA: return 4;
			case OF_IMAGE_COLOR: return 3;
			case OF_IMAGE_GRAYSCALE: default: return 1;
		}
	}
	inline int getChannels(Mat& mat) {
		return mat.channels();
	}
	template <class T> inline int getChannels(ofPixels_<T>& pixels) {
		return pixels.getNumChannels();
	}
	template <class T> inline int getChannels(ofBaseHasPixels_<T>& img) {
		return getChannels(img.getPixelsRef());
	}
	
	// image type
	inline int getCvImageType(ofImageType imageType, int depth = CV_8U) {
		return CV_MAKETYPE(depth, getChannels(imageType));
	}
	template <class T> inline int getCvImageType(T& img) {
		return CV_MAKETYPE(getDepth(img), getChannels(img));
	}
	inline ofImageType getOfImageType(int cvImageType) {
		switch(getChannels(cvImageType)) {
			case 4: return OF_IMAGE_COLOR_ALPHA;
			case 3: return OF_IMAGE_COLOR;
			case 1: default: return OF_IMAGE_GRAYSCALE;
		}
	}
	template <class T> inline ofImageType getOfImageType(T& img) {
		switch(getChannels(img)) {
			case 4: return OF_IMAGE_COLOR_ALPHA;
			case 3: return OF_IMAGE_COLOR;
			case 1: default: return OF_IMAGE_GRAYSCALE;
		}
	}
	
	// allocationg
	template <class T> inline void allocate(T& img, int width, int height, int cvType) {
		img.allocate(width, height, getOfImageType(cvType));
	}
	inline void allocate(Mat& img, int width, int height, int cvType) {
		img.create(height, width, cvType);
	}
	
	// imitate() is good for preparing buffers
	// it's like allocate(), but uses the size and type of the original as a reference
	// should imitate()'s arguments be switched to mirror every other as src/dst?
	template <class M, class O> void imitate(M& mirror, O& original) {
		int mw = getWidth(mirror);
		int mh = getHeight(mirror);
		int ow = getWidth(original);
		int oh = getHeight(original);
		int mt = getCvImageType(mirror);
		int ot = getCvImageType(original);
		if(mw != ow || mh != oh || mt != ot) {
			allocate(mirror, ow, oh, ot);
		}
	}
	
	// this version of imitate() is used for copying the size only
	template <class M, class O> void imitate(M& mirror, O& original, int targetCvImageType) {
		int mw = getWidth(mirror);
		int mh = getHeight(mirror);
		int ow = getWidth(original);
		int oh = getHeight(original);
		int mt = getCvImageType(mirror);
		if(mw != ow || mh != oh || mt != targetCvImageType) {
			allocate(mirror, ow, oh, targetCvImageType);
		}
	}
	
	// cross-toolkit copying
	template <class S, class D>
	void copy(S& src, D& dst) {
		imitate(dst, src);
		Mat srcMat = toCv(src);
		Mat dstMat = toCv(dst);
		srcMat.copyTo(dstMat);
	}

	// maximum possible values for that depth or matrix
	float getMaxVal(int depth);
	float getMaxVal(const Mat& mat);
	int getTargetChannelsFromCode(int conversionCode);
	
	// 2 toCv-compatible wrappers
	
	// wrapThree are based on functions that operate on three Mat objects.
	// the first two are inputs, and the third is an output. for example,
	// the min() function: min(x, y, result) will calculate the per-element min
	// between x and y, and store that in result.
#define wrapThree(name) \
template <class X, class Y, class Result>\
void name(X& x, Y& y, Result& result) {\
imitate(result, x);\
Mat xMat = toCv(x);\
Mat yMat = toCv(y);\
Mat resultMat = toCv(result);\
cv::name(xMat, yMat, resultMat);\
}
	wrapThree(max);
	wrapThree(min);
	wrapThree(multiply);
	wrapThree(divide);
	wrapThree(add);
	wrapThree(subtract);
	wrapThree(absdiff);
	wrapThree(bitwise_and);
	wrapThree(bitwise_or);
	wrapThree(bitwise_xor);
	
	// also useful for taking the average/mixing two images
	template <class X, class Y, class R>
	void lerp(X& x, Y& y, R& result, float amt = .5) {
		imitate(result, x);
		Mat xMat = toCv(x);
		Mat yMat = toCv(y);
		Mat resultMat = toCv(result);
		cv::addWeighted(xMat, amt, yMat, 1. - amt, 0., resultMat);
	}
	
	// normalize the min/max to [0, max for this type] out of place
	template <class S, class D>
	void normalize(S& src, D& dst) {
		imitate(dst, src);
		Mat srcMat = toCv(src);
		Mat dstMat = toCv(dst);
		cv::normalize(srcMat, dstMat, 0, getMaxVal(getDepth(dst)), NORM_MINMAX);
	}
	
	// normalize the min/max to [0, max for this type] in place
	template <class SD>
	void normalize(SD& srcDst) {
		normalize(srcDst, srcDst);
	}
	
	// threshold out of place
	template <class S, class D>
	void threshold(S& src, D& dst, float thresholdValue, bool invert = false) {
		imitate(dst, src);
		Mat srcMat = toCv(src);
		Mat dstMat = toCv(dst);
		int thresholdType = invert ? THRESH_BINARY_INV : THRESH_BINARY;
		float maxVal = getMaxVal(dstMat);
		cv::threshold(srcMat, dstMat, thresholdValue, maxVal, thresholdType);
	}
	
	// threshold in place
	template <class SD>
	void threshold(SD& srcDst, float thresholdValue, bool invert = false) {
		ofxCv::threshold(srcDst, srcDst, thresholdValue, invert);
	}
	
	// CV_RGB2GRAY, CV_HSV2RGB, etc. with [RGB, BGR, GRAY, HSV, HLS, XYZ, YCrCb, Lab, Luv]
	template <class S, class D>
	void convertColor(S& src, D& dst, int code) {
		// cvtColor allocates Mat for you, but we need this to handle ofImage etc.
		int targetChannels = getTargetChannelsFromCode(code);
		imitate(dst, src, getCvImageType(targetChannels));
		Mat srcMat = toCv(src);
		Mat dstMat = toCv(dst);
		cvtColor(srcMat, dstMat, code);
	}
	
	// older wrappers, need to be templated
	// {
	
	// for contourArea()/arcLength(), see ofPolyline::getArea()/getPerimiter()
	// not sure if these three need to be templated. convexHull returning an
	// ofPolyline when given an ofPolyline is the key factor...
	ofPolyline convexHull(ofPolyline& polyline);
	cv::RotatedRect minAreaRect(ofPolyline& polyline);
	cv::RotatedRect fitEllipse(ofPolyline& polyline);
	
	void invert(ofImage& img);
	void rotate(ofImage& source, ofImage& destination, double angle, unsigned char fill = 0, int interpolation = INTER_LINEAR);
	void autorotate(ofImage& original, ofImage& thresh, ofImage& output, float* rotation = NULL);
	void autothreshold(ofImage& original, ofImage& thresh, bool invert = false);
	void autothreshold(ofImage& original, bool invert = false);
	//void threshold(FloatImage& img, float value, bool invert = false);
	//void threshold(FloatImage& original, FloatImage& thresh, float value, bool invert = false);
	//void matchRegion(ofImage& source, ofRectangle& region, ofImage& search, FloatImage& result);
	void matchRegion(Mat& source, ofRectangle& region, Mat& search, Mat& result);
	//void convolve(ofImage& source, FloatImage& kernel, ofImage& destination);
	//void convolve(ofImage& img, FloatImage& kernel);
	//void blur(FloatImage& original, FloatImage& blurred, int size);
	void medianBlur(ofImage& img, int size);
	void warpPerspective(ofImage& src, ofImage& dst, Mat& m, int flags = 0);
	void warpPerspective(ofPixels& src, ofPixels& dst, Mat& m, int flags = 0);
	void resize(ofImage& source, ofImage& destination, int interpolation = INTER_LINEAR); // options: INTER_NEAREST, INTER_LINEAR, INTER_AREA, INTER_CUBIC, INTER LANCZOS4
	void resize(ofImage& source, ofImage& destination, float xScale, float yScale, int interpolation = INTER_LINEAR);
	
	// }
	
	// 3 misc
	void loadImage(Mat& mat, string filename);
	void saveImage(Mat mat, string filename);
	
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
	
	float weightedAverageAngle(const vector<Vec4i>& lines);
	
	
	void drawHighlightString(string text, int x, int y, ofColor background = ofColor::black, ofColor foreground = ofColor::white);
}

// <3 kyle
