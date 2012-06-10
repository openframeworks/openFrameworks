/*
 wrappers provide an easy-to-use interface to OpenCv functions when using data
 from openFrameworks. they don't implement anything novel, they just wrap OpenCv
 functions in a very direct way. many of the functions have in-place and
 not-in-place variations.
 
 high level image operations:
 - Canny (edge detection), medianBlur, blur (gaussian), convertColor
 
 low level image manipulation and comparison:
 - threshold, normalize, invert, lerp
 - bitwise_and, bitwise_or, bitwise_xor
 - max, min, multiply, divide, add, subtract, absdiff
 
 image transformation:
 - rotate, resize, warpPerspective
 
 point set/ofPolyline functions:
 - convexHull, minAreaRect, fitEllipse, unwarpPerspective, warpPerspective
 
 utility wrappers:
 - load and save Mat
 
 */

#pragma once

#include "ofMain.h"
#include "opencv2/opencv.hpp"
#include "ofxCv/Utilities.h"

namespace ofxCv {
	
	using namespace cv;
	
	void loadMat(Mat& mat, string filename);
	void saveMat(Mat mat, string filename);
	
	// wrapThree are based on functions that operate on three Mat objects.
	// the first two are inputs, and the third is an output. for example,
	// the min() function: min(x, y, result) will calculate the per-element min
	// between x and y, and store that in result. both y and result need to
	// match x in dimensions and type. while wrapThree functions will use
	// imitate() to make sure your data is allocated correctly, you shouldn't
	// epect the function to behave properly if you haven't already allocated
	// your y argument. in general, OF images contain noise when newly allocated
	// so the result will also contain that noise.
#define wrapThree(name) \
template <class X, class Y, class Result>\
void name(X& x, Y& y, Result& result) {\
imitate(y, x);\
imitate(result, x);\
Mat xMat = toCv(x), yMat = toCv(y);\
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
	
	// inverting non-floating point images is a just a bitwise not operation
	template <class S, class D> void invert(S& src, D& dst) {
		Mat srcMat = toCv(src), dstMat = toCv(dst);
		bitwise_not(srcMat, dstMat);
	}
	
	template <class SD> void invert(SD& srcDst) {
		ofxCv::invert(srcDst, srcDst);
	}
	
	// also useful for taking the average/mixing two images
	template <class X, class Y, class R>
	void lerp(X& x, Y& y, R& result, float amt = .5) {
		imitate(result, x);
		Mat xMat = toCv(x), yMat = toCv(y);
		Mat resultMat = toCv(result);
		if(yMat.cols == 0) {
			copy(x, result);
		} else if(xMat.cols == 0) {
			copy(y, result);
		} else {
			cv::addWeighted(xMat, amt, yMat, 1. - amt, 0., resultMat);
		}
	}
	
	// normalize the min/max to [0, max for this type] out of place
	template <class S, class D>
	void normalize(S& src, D& dst) {
		imitate(dst, src);
		Mat srcMat = toCv(src), dstMat = toCv(dst);
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
		Mat srcMat = toCv(src), dstMat = toCv(dst);
		int thresholdType = invert ? THRESH_BINARY_INV : THRESH_BINARY;
		float maxVal = getMaxVal(dstMat);
		cv::threshold(srcMat, dstMat, thresholdValue, maxVal, thresholdType);
	}
	
	// threshold in place
	template <class SD>
	void threshold(SD& srcDst, float thresholdValue, bool invert = false) {
		ofxCv::threshold(srcDst, srcDst, thresholdValue, invert);
	}
	
	// automatic threshold (grayscale 8-bit only) out of place
	template <class S, class D>
	void autothreshold(S& src, D& dst, bool invert = false) {
		imitate(dst, src);
		Mat srcMat = toCv(src), dstMat = toCv(dst);
		int flags = THRESH_OTSU | (invert ? THRESH_BINARY_INV : THRESH_BINARY);
		threshold(srcMat, dstMat, 0, 255, flags);
	}
	
	// automatic threshold (grayscale 8-bit only) in place
	template <class SD>
	void autothreshold(SD& srcDst, bool invert = false) {
		ofxCv::autothreshold(srcDst, srcDst, invert);
	}
	
	// CV_RGB2GRAY, CV_HSV2RGB, etc. with [RGB, BGR, GRAY, HSV, HLS, XYZ, YCrCb, Lab, Luv]
	// you can convert whole images...
	template <class S, class D>
	void convertColor(S& src, D& dst, int code) {
		// cvtColor allocates Mat for you, but we need this to handle ofImage etc.
		int targetChannels = getTargetChannelsFromCode(code);
		imitate(dst, src, getCvImageType(targetChannels, getDepth(src)));
		Mat srcMat = toCv(src), dstMat = toCv(dst);
		cvtColor(srcMat, dstMat, code);
	}
	// ...or single colors.
	Vec3b convertColor(Vec3b color, int code);
	ofColor convertColor(ofColor color, int code);
	
	int forceOdd(int x);
	
	// Gaussian blur
	template <class S, class D>
	void blur(S& src, D& dst, int size) {
		imitate(dst, src);
		size = forceOdd(size);
		Mat srcMat = toCv(src), dstMat = toCv(dst);
		cv::GaussianBlur(srcMat, dstMat, cv::Size(size, size), 0, 0);
	}
	
	// in-place Gaussian blur
	template <class SD>
	void blur(SD& srcDst, int size) {
		ofxCv::blur(srcDst, srcDst, size);
	}
	
	// Median blur
	template <class S, class D>
	void medianBlur(S& src, D& dst, int size) {
		imitate(dst, src);
		size = forceOdd(size);
		Mat srcMat = toCv(src), dstMat = toCv(dst);
		cv::medianBlur(srcMat, dstMat, size);
	}
	
	// in-place Median blur
	template <class SD>
	void medianBlur(SD& srcDst, int size) {
		ofxCv::medianBlur(srcDst, srcDst, size);
	}
	
	// Canny edge detection assumes your input and output are grayscale 8-bit
	// example thresholds might be 0,30 or 50,200
	template <class S, class D>
	void Canny(S& src, D& dst, double threshold1, double threshold2, int apertureSize=3, bool L2gradient=false) {
		imitate(dst, src, CV_8UC1);
		Mat srcMat = toCv(src), dstMat = toCv(dst);
		cv::Canny(srcMat, dstMat, threshold1, threshold2, apertureSize, L2gradient);
	}	
	
	// dst does not imitate src
	template <class S, class D>
	void warpPerspective(S& src, D& dst, vector<Point2f>& dstPoints, int flags = INTER_LINEAR) {
		Mat srcMat = toCv(src), dstMat = toCv(dst);
		int w = srcMat.cols;
		int h = srcMat.rows;
		vector<Point2f> srcPoints(4);
		srcPoints[0] = Point2f(0, 0);
		srcPoints[1] = Point2f(w, 0);
		srcPoints[2] = Point2f(w, h);
		srcPoints[3] = Point2f(0, h);
		Mat transform = getPerspectiveTransform(&srcPoints[0], &dstPoints[0]);
		warpPerspective(srcMat, dstMat, transform, dstMat.size(), flags);
	}
	
	// dst does not imitate src
	template <class S, class D>
	void unwarpPerspective(S& src, D& dst, vector<Point2f>& srcPoints, int flags = INTER_LINEAR) {
		Mat srcMat = toCv(src), dstMat = toCv(dst);
		int w = dstMat.cols;
		int h = dstMat.rows;
		vector<Point2f> dstPoints(4);
		dstPoints[0] = Point2f(0, 0);
		dstPoints[1] = Point2f(w, 0);
		dstPoints[2] = Point2f(w, h);
		dstPoints[3] = Point2f(0, h);
		Mat transform = getPerspectiveTransform(&srcPoints[0], &dstPoints[0]);
		warpPerspective(srcMat, dstMat, transform, dstMat.size(), flags);
	}
	
	// dst does not imitate src
	template <class S, class D>
	void warpPerspective(S& src, D& dst, Mat& transform, int flags = INTER_LINEAR) {
		Mat srcMat = toCv(src), dstMat = toCv(dst);
		warpPerspective(srcMat, dstMat, transform, dstMat.size(), flags);
	}
	
	template <class S, class D>
	void resize(S& src, D& dst, int interpolation = INTER_LINEAR) { // also: INTER_NEAREST, INTER_AREA, INTER_CUBIC, INTER_LANCZOS4
		Mat srcMat = toCv(src), dstMat = toCv(dst);
		resize(srcMat, dstMat, dstMat.size(), 0, 0, interpolation);
	}
	
	template <class S, class D>
	void resize(S& src, D& dst, float xScale, float yScale, int interpolation = INTER_LINEAR) { // also: INTER_NEAREST, INTER_AREA, INTER_CUBIC, INTER_LANCZOS4
		allocate(dst, getWidth(src) * xScale, getHeight(src) * yScale, getCvImageType(src));
		Mat srcMat = toCv(src), dstMat = toCv(dst);
		resize(src, dst, interpolation);
	}
	
	// for contourArea() and arcLength(), see ofPolyline::getArea() and getPerimiter()
	ofPolyline convexHull(const ofPolyline& polyline);
	cv::RotatedRect minAreaRect(const ofPolyline& polyline);
	cv::RotatedRect fitEllipse(const ofPolyline& polyline);
	
	// kind of obscure function, draws filled polygons on the CPU
	template <class D>
	void fillPoly(vector<cv::Point>& points, D& dst) {
		cv::Mat dstMat = toCv(dst);
		const cv::Point* ppt[1] = { &(points[0]) };
		int npt[] = { points.size() };
		dstMat.setTo(Scalar(0));
		fillPoly(dstMat, ppt, npt, 1, Scalar(255));
	}
	
	// if you're doing the same rotation multiple times, it's better to precompute
	// the displacement and use remap.
	template <class S, class D>
	void rotate(S& src, D& dst, double angle, ofColor fill = ofColor::black, int interpolation = INTER_LINEAR) {
		imitate(dst, src);
		Mat srcMat = toCv(src), dstMat = toCv(dst);
		Point2f center(srcMat.rows / 2, srcMat.cols / 2);
		Mat rotationMatrix = getRotationMatrix2D(center, angle, 1);
		warpAffine(srcMat, dstMat, rotationMatrix, srcMat.size(), interpolation, BORDER_CONSTANT, toCv(fill));
	}
}
