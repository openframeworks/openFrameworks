/*
 wrappers provide an easy-to-use interface to OpenCv functions when using data
 from openFrameworks. they don't implement anything novel, they just wrap OpenCv
 functions in a very direct way. many of the functions have in-place and
 not-in-place variations.

 high level image operations:
 - Canny (edge detection), medianBlur, blur, convertColor
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

#include "opencv2/opencv.hpp"
#include "ofxCv/Utilities.h"
#include "ofVectorMath.h"
#include "ofImage.h"

// coherent line drawing
#include "imatrix.h"
#include "ETF.h"
#include "fdog.h"
#include "myvec.h"

namespace ofxCv {

	void loadMat(cv::Mat& mat, std::string filename);
	void saveMat(cv::Mat mat, std::string filename);
	void saveImage(cv::Mat& mat, std::string filename, ofImageQualityType qualityLevel = OF_IMAGE_QUALITY_BEST);

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
cv::Mat xMat = toCv(x), yMat = toCv(y);\
cv::Mat resultMat = toCv(result);\
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
		cv::Mat srcMat = toCv(src), dstMat = toCv(dst);
		bitwise_not(srcMat, dstMat);
	}

	template <class SD> void invert(SD& srcDst) {
		ofxCv::invert(srcDst, srcDst);
	}

	// also useful for taking the average/mixing two images
	template <class X, class Y, class R>
	void lerp(X& x, Y& y, R& result, float amt = .5) {
		imitate(result, x);
		cv::Mat xMat = toCv(x), yMat = toCv(y);
		cv::Mat resultMat = toCv(result);
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
		cv::Mat srcMat = toCv(src), dstMat = toCv(dst);
		cv::normalize(srcMat, dstMat, 0, getMaxVal(getDepth(dst)), cv::NORM_MINMAX);
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
		cv::Mat srcMat = toCv(src), dstMat = toCv(dst);
		int thresholdType = invert ? cv::THRESH_BINARY_INV : cv::THRESH_BINARY;
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
		cv::Mat srcMat = toCv(src), dstMat = toCv(dst);
		cv::erode(srcMat, dstMat, cv::Mat(), cv::Point(-1, -1), iterations);
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
		cv::Mat srcMat = toCv(src), dstMat = toCv(dst);
		cv::dilate(srcMat, dstMat, cv::Mat(), cv::Point(-1, -1), iterations);
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
		cv::Mat srcMat = toCv(src), dstMat = toCv(dst);
		int flags = cv::THRESH_OTSU | (invert ? cv::THRESH_BINARY_INV : cv::THRESH_BINARY);
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
		cv::Mat srcMat = toCv(src), dstMat = toCv(dst);
		cvtColor(srcMat, dstMat, code);
	}
	// ...or single colors.
	cv::Vec3b convertColor(cv::Vec3b color, int code);
	ofColor convertColor(ofColor color, int code);

    // a common cv task is to convert something to grayscale. this function will
    // do that quickly for RGBA, RGB, and 1-channel images.
    template <class S, class D>
    void copyGray(S& src, D& dst) {
        int channels = getChannels(src);
        if(channels == 4) {
            convertColor(src, dst, CV_RGBA2GRAY);
        } else if(channels == 3) {
            convertColor(src, dst, CV_RGB2GRAY);
        } else if(channels == 1) {
            copy(src, dst);
        }
    }

	int forceOdd(int x);

	// box blur
	template <class S, class D>
	void blur(S& src, D& dst, int size) {
		imitate(dst, src);
		size = forceOdd(size);
		cv::Mat srcMat = toCv(src), dstMat = toCv(dst);
		cv::blur(srcMat, dstMat, cv::Size(size, size));
	}

	// in-place box blur
	template <class SD>
	void blur(SD& srcDst, int size) {
		ofxCv::blur(srcDst, srcDst, size);
	}

    // Gaussian blur
    template <class S, class D>
    void GaussianBlur(S& src, D& dst, int size) {
        imitate(dst, src);
        size = forceOdd(size);
		cv::Mat srcMat = toCv(src), dstMat = toCv(dst);
        cv::GaussianBlur(srcMat, dstMat, cv::Size(size, size), 0, 0);
    }

    // in-place Gaussian blur
    template <class SD>
    void GaussianBlur(SD& srcDst, int size) {
        ofxCv::GaussianBlur(srcDst, srcDst, size);
    }

	// Median blur
	template <class S, class D>
	void medianBlur(S& src, D& dst, int size) {
		imitate(dst, src);
		size = forceOdd(size);
		cv::Mat srcMat = toCv(src), dstMat = toCv(dst);
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
		cv::Mat srcMat = toCv(src), dstMat = toCv(dst);
		if(srcMat.channels() > 1) {
			std::vector<cv::Mat> srcEach, dstEach;
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
		cv::Mat srcMat = toCv(src), dstMat = toCv(dst);
		cv::Canny(srcMat, dstMat, threshold1, threshold2, apertureSize, L2gradient);
	}

	// Sobel edge detection
	template <class S, class D>
	void Sobel(S& src, D& dst, int ddepth=-1, int dx=1, int dy=1, int ksize=3, double scale=1, double delta=0, int borderType=cv::BORDER_DEFAULT ) {
		imitate(dst, src, CV_8UC1);
		cv::Mat srcMat = toCv(src), dstMat = toCv(dst);
		cv::Sobel(srcMat, dstMat, ddepth, dx, dy, ksize, scale, delta, borderType );
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
		cv::Mat dstMat = toCv(dst);
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
	void warpPerspective(S& src, D& dst, std::vector<cv::Point2f>& dstPoints, int flags = cv::INTER_LINEAR) {
		cv::Mat srcMat = toCv(src), dstMat = toCv(dst);
		int w = srcMat.cols;
		int h = srcMat.rows;
		std::vector<cv::Point2f> srcPoints(4);
		srcPoints[0] = cv::Point2f(0, 0);
		srcPoints[1] = cv::Point2f(w, 0);
		srcPoints[2] = cv::Point2f(w, h);
		srcPoints[3] = cv::Point2f(0, h);
		cv::Mat transform = getPerspectiveTransform(&srcPoints[0], &dstPoints[0]);
		warpPerspective(srcMat, dstMat, transform, dstMat.size(), flags);
	}

	// dst does not imitate src
	template <class S, class D>
	void unwarpPerspective(S& src, D& dst, std::vector<cv::Point2f>& srcPoints, int flags = cv::INTER_LINEAR) {
		cv::Mat srcMat = toCv(src), dstMat = toCv(dst);
		int w = dstMat.cols;
		int h = dstMat.rows;
		std::vector<cv::Point2f> dstPoints(4);
		dstPoints[0] = cv::Point2f(0, 0);
		dstPoints[1] = cv::Point2f(w, 0);
		dstPoints[2] = cv::Point2f(w, h);
		dstPoints[3] = cv::Point2f(0, h);
		cv::Mat transform = getPerspectiveTransform(&srcPoints[0], &dstPoints[0]);
		warpPerspective(srcMat, dstMat, transform, dstMat.size(), flags);
	}

	// dst does not imitate src
	template <class S, class D>
	void warpPerspective(S& src, D& dst, cv::Mat& transform, int flags = cv::INTER_LINEAR) {
		cv::Mat srcMat = toCv(src), dstMat = toCv(dst);
		warpPerspective(srcMat, dstMat, transform, dstMat.size(), flags);
	}

	template <class S, class D>
	void resize(S& src, D& dst, int interpolation = cv::INTER_LINEAR) { // also: INTER_NEAREST, INTER_AREA, INTER_CUBIC, INTER_LANCZOS4
		cv::Mat srcMat = toCv(src), dstMat = toCv(dst);
		resize(srcMat, dstMat, dstMat.size(), 0, 0, interpolation);
	}

	template <class S, class D>
	void resize(S& src, D& dst, float xScale, float yScale, int interpolation = cv::INTER_LINEAR) { // also: INTER_NEAREST, INTER_AREA, INTER_CUBIC, INTER_LANCZOS4
		int dstWidth = getWidth(src) * xScale, dstHeight = getHeight(src) * yScale;
		if(getWidth(dst) != dstWidth || getHeight(dst) != dstHeight) {
			allocate(dst, dstWidth, dstHeight, getCvImageType(src));
		}
		cv::Mat srcMat = toCv(src), dstMat = toCv(dst);
		resize(src, dst, interpolation);
	}

	// for contourArea() and arcLength(), see ofPolyline::getArea() and getPerimiter()
	ofPolyline convexHull(const ofPolyline& polyline);
	std::vector<cv::Vec4i> convexityDefects(const std::vector<cv::Point>& contour);
	std::vector<cv::Vec4i> convexityDefects(const ofPolyline& polyline);
	cv::RotatedRect minAreaRect(const ofPolyline& polyline);
	cv::RotatedRect fitEllipse(const ofPolyline& polyline);
	void fitLine(const ofPolyline& polyline, ofVec2f& point, ofVec2f& direction);

	// kind of obscure function, draws filled polygons on the CPU
	template <class D>
	void fillPoly(std::vector<cv::Point>& points, D& dst) {
		cv::Mat dstMat = toCv(dst);
		const cv::Point* ppt[1] = { &(points[0]) };
		int npt[] = { (int) points.size() };
		dstMat.setTo(cv::Scalar(0));
		fillPoly(dstMat, ppt, npt, 1, cv::Scalar(255));
	}

	template <class S, class D>
	void flip(S& src, D& dst, int code) {
		imitate(dst, src);
		cv::Mat srcMat = toCv(src), dstMat = toCv(dst);
		cv::flip(srcMat, dstMat, code);
	}

	// if you're doing the same rotation multiple times, it's better to precompute
	// the displacement and use remap.
	template <class S, class D>
	void rotate(S& src, D& dst, double angle, ofColor fill = ofColor::black, int interpolation = cv::INTER_LINEAR) {
		imitate(dst, src);
		cv::Mat srcMat = toCv(src), dstMat = toCv(dst);
		cv::Point2f center(srcMat.cols / 2, srcMat.rows / 2);
		cv::Mat rotationMatrix = getRotationMatrix2D(center, angle, 1);
		warpAffine(srcMat, dstMat, rotationMatrix, srcMat.size(), interpolation, cv::BORDER_CONSTANT, toCv(fill));
	}

	// efficient version of rotate that only operates on 0, 90, 180, 270 degrees
	// the output is allocated to contain all pixels of the input.
	template <class S, class D>
	void rotate90(S& src, D& dst, int angle) {
		cv::Mat srcMat = toCv(src), dstMat = toCv(dst);
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

    template <class S, class D>
    void transpose(S& src, D& dst) {
		cv::Mat srcMat = toCv(src);
        allocate(dst, srcMat.rows, srcMat.cols, srcMat.type());
		cv::Mat dstMat = toCv(dst);
        cv::transpose(srcMat, dstMat);
    }

	// finds the 3x4 matrix that best describes the (premultiplied) affine transformation between two point clouds
	ofMatrix4x4 estimateAffine3D(std::vector<ofVec3f>& from, std::vector<ofVec3f>& to, float accuracy = .99);
	ofMatrix4x4 estimateAffine3D(std::vector<ofVec3f>& from, std::vector<ofVec3f>& to, std::vector<unsigned char>& outliers, float accuracy = .99);
}
