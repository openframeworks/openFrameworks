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
		
	// toOf functions
	ofVec2f toOf(Point2f point);
	ofVec3f toOf(Point3f point);
	ofRectangle toOf(cv::Rect rect);
	
	// these functions are for accessing Mat, ofPixels and ofImage consistently
	// the number of functions could be reduced and the dependency simplified a little
	// they're important for imitate(), so essentially:
	// get width, height, channels, depth, and image type. do allocation.
	template <class T> inline int getWidth(T& src) {return src.getWidth();}
	template <class T> inline int getHeight(T& src) {return src.getHeight();}
	template <> inline int getWidth<Mat>(Mat& src) {return src.cols;}
	template <> inline int getHeight<Mat>(Mat& src) {return src.rows;}
	
	template <class T> inline int getDepth(T& img);
	template <> inline int getDepth<Mat>(Mat& mat) {return mat.depth();}
	template <> inline int getDepth<ofImage>(ofImage& img) {return CV_8U;}
	template <> inline int getDepth<ofShortImage>(ofShortImage& img) {return CV_16U;}
	template <> inline int getDepth<ofFloatImage>(ofFloatImage& img) {return CV_32F;}
	template <> inline int getDepth<ofPixels>(ofPixels& img) {return CV_8U;}
	template <> inline int getDepth<ofShortPixels>(ofShortPixels& img) {return CV_16U;}
	template <> inline int getDepth<ofFloatPixels>(ofFloatPixels& img) {return CV_32F;}
	
	int getChannels(const ofImageType& ofType);
	
	template <class T> inline ofImageType getOfImageType(T& img);
	template <class T> inline ofImageType getOfImageType(ofPixels_<T>& pixels) {return pixels.getImageType();}
	template <class T> inline ofImageType getOfImageType(ofImage_<T>& image) {return getOfImageType(image.getPixelsRef());}
	ofImageType getOfImageType(int channels);
	template <> inline ofImageType getOfImageType<Mat>(Mat& mat) {return getOfImageType(mat.channels());}
	int getCvImageType(int channels, int depth = CV_8U);
	template <class T> inline int getCvImageType(T& img) {return getCvImageType(getChannels(getOfImageType(img)), getDepth(img));}
	template <> inline int getCvImageType(Mat& mat) {return mat.type();}
	
	template <class T> inline void allocate(T& img, int width, int height, int cvType) {img.allocate(width, height, getOfImageType(cvType));}
	template <> inline void allocate<Mat>(Mat& img, int width, int height, int cvType) {img.create(height, width, cvType);}
	
	// imitate() is good for preparing buffers
	// it's like allocate(), but uses the size and type of the original as a reference
	// imitate() assumes that you want your mirror Mat to be CV_8U right now
	// should imitate()'s arguments be switched to mirror every other as src/dst?
	template <class M, class O> void imitate(M& mirror, O& original) {
		int mw = getWidth(mirror);
		int mh = getHeight(mirror);
		int ow = getWidth(original);
		int oh = getHeight(original);
		ofImageType mt = getOfImageType(mirror);
		ofImageType ot = getOfImageType(original);
		if(mw != ow || mh != oh || mt != ot) {
			int cvType = getCvImageType(original);
			allocate(mirror, ow, oh, cvType);
		}
	}
	
	// this version of imitate() is used for copying the size only
	template <class M, class O> void imitate(M& mirror, O& original, int cvImageType) {
		int mw = getWidth(mirror);
		int mh = getHeight(mirror);
		int ow = getWidth(original);
		int oh = getHeight(original);
		int mt = getCvImageType(mirror);
		if(mw != ow || mh != oh || mt != cvImageType) {
			allocate(mirror, ow, oh, cvImageType);
		}
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
}

// <3 kyle
