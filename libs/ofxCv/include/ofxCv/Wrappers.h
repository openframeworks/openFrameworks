/*
 wrappers provide an easy-to-use interface to OpenCv functions when using data
 from openFrameworks. they don't implement anything novel, they just wrap OpenCv
 functions in a very direct way. many of the functions have in-place and
 not-in-place variations.
 
 high level image operations:
 - Canny (edge detection), medianBlur, blur (gaussian), convertColor
 - Coherent line drawing
 
 low level image manipulation and comparison:
 - threshold, normalize, invert, lerp
 - bitwise_and, bitwise_or, bitwise_xor
 - max, min, multiply, divide, add, subtract, absdiff
 - erode, dilate
 
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

// coherent line drawing
#include "imatrix.h"
#include "ETF.h"
#include "fdog.h"
#include "myvec.h"

namespace ofxCv {
	
	using namespace cv;
	
	void loadMat(Mat& mat, string filename);
	void saveMat(Mat mat, string filename);
	void saveImage(Mat& mat, string filename);
	
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
	
	// erode out of place
	template <class S, class D>
	void erode(S& src, D& dst, int iterations = 1) {
		imitate(dst, src);
		Mat srcMat = toCv(src), dstMat = toCv(dst);
		cv::erode(srcMat, dstMat, Mat(), cv::Point(-1, -1), iterations);
	}
	
	// erode in place
	template <class SD>
	void erode(SD& srcDst, int iterations = 1) {
		ofxCv::erode(srcDst, srcDst, iterations);
	}
	
	// dilate out of place
	template <class S, class D>
	void dilate(S& src, D& dst, int iterations = 1) {
		imitate(dst, src);
		Mat srcMat = toCv(src), dstMat = toCv(dst);
		cv::dilate(srcMat, dstMat, Mat(), cv::Point(-1, -1), iterations);
	}
	
	// dilate in place
	template <class SD>
	void dilate(SD& srcDst, int iterations = 1) {
		ofxCv::dilate(srcDst, srcDst, iterations);
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
	
	// histogram equalization, adds support for color images
	template <class S, class D>
	void equalizeHist(S& src, D& dst) {
		imitate(dst, src);
		Mat srcMat = toCv(src), dstMat = toCv(dst);
		if(srcMat.channels() > 1) {
			vector<Mat> srcEach, dstEach;
			split(srcMat, srcEach);
			split(dstMat, dstEach);
			for(int i = 0; i < srcEach.size(); i++) {
				cv::equalizeHist(srcEach[i], dstEach[i]);
			}
			cv::merge(dstEach, dstMat);
		} else {
			cv::equalizeHist(srcMat, dstMat);
		}
	}
	
	// in-place histogram equalization
	template <class SD>
	void equalizeHist(SD& srcDst) {
		equalizeHist(srcDst, srcDst);
	}
	
	// Canny edge detection assumes your input and output are grayscale 8-bit
	// example thresholds might be 0,30 or 50,200
	template <class S, class D>
	void Canny(S& src, D& dst, double threshold1, double threshold2, int apertureSize=3, bool L2gradient=false) {
		imitate(dst, src, CV_8UC1);
		Mat srcMat = toCv(src), dstMat = toCv(dst);
		cv::Canny(srcMat, dstMat, threshold1, threshold2, apertureSize, L2gradient);
	}
	
	// coherent line drawing: good values for halfw are between 1 and 8,
	// smoothPasses 1, and 4, sigma1 between .01 and 2, sigma2 between .01 and 10,
	// tau between .8 and 1.0
	// this could be rewritten into a class so we're not doing an allocate and copy each time
	template <class S, class D>
	void CLD(S& src, D& dst, int halfw = 4, int smoothPasses = 2, double sigma1 = .4, double sigma2 = 3, double tau = .97, int black = 0) {
		copy(src, dst);
		int width = getWidth(src), height = getHeight(src);
		imatrix img;
		img.init(height, width);
		Mat dstMat = toCv(dst);
		if(black != 0) {
			add(dstMat, cv::Scalar(black), dstMat);
		}
		// copy from dst (unsigned char) to img (int)
		for(int y = 0; y < height; y++) {
			for(int x = 0; x < width; x++) {
				img[y][x] = dstMat.at<unsigned char>(y, x);
			}
		}
		ETF etf;
		etf.init(height, width);
		etf.set(img);
		etf.Smooth(halfw, smoothPasses);
		GetFDoG(img, etf, sigma1, sigma2, tau);
		// copy result from img (int) to dst (unsigned char)
		for(int y = 0; y < height; y++) {
			for(int x = 0; x < width; x++) {
				dstMat.at<unsigned char>(y, x) = img[y][x];
			}
		}
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
		int dstWidth = getWidth(src) * xScale, dstHeight = getHeight(src) * yScale;
		if(getWidth(dst) != dstWidth || getHeight(dst) != dstHeight) {
			allocate(dst, dstWidth, dstHeight, getCvImageType(src));
		}
		Mat srcMat = toCv(src), dstMat = toCv(dst);
		resize(src, dst, interpolation);
	}
	
	// for contourArea() and arcLength(), see ofPolyline::getArea() and getPerimiter()
	ofPolyline convexHull(const ofPolyline& polyline);
	vector<cv::Vec4i> convexityDefects(const vector<cv::Point>& contour);
	vector<cv::Vec4i> convexityDefects(const ofPolyline& polyline);
	cv::RotatedRect minAreaRect(const ofPolyline& polyline);
	cv::RotatedRect fitEllipse(const ofPolyline& polyline);
	void fitLine(const ofPolyline& polyline, ofVec2f& point, ofVec2f& direction);
	
	// kind of obscure function, draws filled polygons on the CPU
	template <class D>
	void fillPoly(vector<cv::Point>& points, D& dst) {
		cv::Mat dstMat = toCv(dst);
		const cv::Point* ppt[1] = { &(points[0]) };
		int npt[] = { points.size() };
		dstMat.setTo(Scalar(0));
		fillPoly(dstMat, ppt, npt, 1, Scalar(255));
	}
	
	template <class S, class D>
	void flip(S& src, D& dst, int code) {
		imitate(dst, src);
		Mat srcMat = toCv(src), dstMat = toCv(dst);
		cv::flip(srcMat, dstMat, code);
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
	
	// efficient version of rotate that only operates on 0, 90, 180, 270 degrees
	// the output is allocated to contain all pixels of the input.
	template <class S, class D>
	void rotate90(S& src, D& dst, int angle) {
		Mat srcMat = toCv(src), dstMat = toCv(dst);
		if(angle == 0) {
			copy(src, dst);
		} else if(angle == 90) {
			allocate(dst, srcMat.rows, srcMat.cols, srcMat.type());
			cv::transpose(srcMat, dstMat);
			cv::flip(dstMat, dstMat, 1);
		} else if(angle == 180) {
			imitate(dst, src);
			cv::flip(srcMat, dstMat, -1);
		} else if(angle == 270) {
			allocate(dst, srcMat.rows, srcMat.cols, srcMat.type());
			cv::transpose(srcMat, dstMat);
			cv::flip(dstMat, dstMat, 0);
		}
	}
	
	// finds the 3x4 matrix that best describes the (premultiplied) affine transformation between two point clouds
	ofMatrix4x4 estimateAffine3D(vector<ofVec3f>& from, vector<ofVec3f>& to, float accuracy = .99);
	ofMatrix4x4 estimateAffine3D(vector<ofVec3f>& from, vector<ofVec3f>& to, vector<unsigned char>& outliers, float accuracy = .99);
}
