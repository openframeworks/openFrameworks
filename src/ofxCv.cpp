#include "ofxCv.h"

#include "ofMath.h"

namespace ofxCv {
	
	using namespace cv;
	
	void imitate(ofImage& mirror, ofImage& original) {
		int w = original.getWidth();
		int h = original.getHeight();
		ofImageType mirrorType = mirror.getPixelsRef().getImageType();
		ofImageType originalType = original.getPixelsRef().getImageType();
		
		if(mirror.bAllocated()) {
			if(mirror.getWidth() != w || mirror.getHeight() != h) {
				mirror.resize(w, h);
			}
			if(mirrorType != originalType) {
				mirror.setImageType(originalType);
			}
		} else {
			mirror.allocate(w, h, originalType);
		}
	}
	
	void imitate(FloatImage& mirror, ofImage& original) {
		int w = original.getWidth();
		int h = original.getHeight();
		
		if(mirror.getWidth() != w || mirror.getHeight() != h) {
			mirror.allocate(w, h);
		}
	}
	
	void imitate(FloatImage& mirror, FloatImage& original) {
		int w = original.getWidth();
		int h = original.getHeight();
		
		if(mirror.getWidth() != w || mirror.getHeight() != h) {
			mirror.allocate(w, h);
		}
	}
	
	int getCvImageType(const ofImageType& ofType) {
		switch(ofType) {
			case OF_IMAGE_GRAYSCALE: return CV_8UC1;
			case OF_IMAGE_COLOR: return CV_8UC3;
			case OF_IMAGE_COLOR_ALPHA: return CV_8UC4;
			default: return CV_8UC1;
		}
	}
	
	Mat getMat(ofImage& img) {
		int cvImageType = getCvImageType(img.getPixelsRef().getImageType());
		return Mat(img.getHeight(), img.getWidth(), cvImageType, img.getPixels(), 0);
	}
	
	ofVec2f makeVec2f(Point2f point) {
		return ofVec2f(point.x, point.y);
	}
	
	cv::Rect makeRect(ofRectangle& rect) {
		return cv::Rect(rect.x, rect.y, rect.width, rect.height);
	}
	
	void invert(ofImage& img) {
		Mat imgMat = getMat(img);
		bitwise_not(imgMat, imgMat);
	}
	
	// if you're doing the same rotation multiple times,
	// it's better to precompute the displacement and use remap
	// fill should be an ofColor, and we need a function to convert ofColor to cv::Scalar
	void rotate(ofImage& original, ofImage& rotated, double angle, unsigned char fill, int interpolation) {
		imitate(rotated, original);
		
		Mat originalMat = getMat(original);
		Mat rotatedMat = getMat(rotated);
		
		Point2f center(originalMat.rows / 2, originalMat.cols / 2);
		Mat rotationMatrix = getRotationMatrix2D(center, angle, 1);
		warpAffine(originalMat, rotatedMat, rotationMatrix, originalMat.size(), interpolation, BORDER_CONSTANT, Scalar(fill));
	}
	
	void autothreshold(ofImage& original, ofImage& thresh, bool invert) {
		imitate(thresh, original);
		
		Mat originalMat = getMat(original);
		Mat threshMat = getMat(thresh);
		
		// this might only work on grayscale atm...
		// if normal thresholding is 400 fps, THRESH_OTSU is 100 fps 
		int flags = THRESH_OTSU | (invert ? THRESH_BINARY_INV : THRESH_BINARY);
		threshold(originalMat, threshMat, 0, 255, flags);
	}
	
	void autothreshold(ofImage& img, bool invert) {
		autothreshold(img, img, invert);
	}
	
	void threshold(FloatImage& img, float value, bool invert) {
		threshold(img, img, value, invert);
	}
	
	void threshold(FloatImage& original, FloatImage& thresh, float value, bool invert) {
		threshold(original.getMat(), thresh.getMat(), value, 1, invert ? THRESH_BINARY_INV : THRESH_BINARY);
	}
	
	void matchRegion(ofImage& source, ofRectangle& region, ofImage& search, FloatImage& result) {
		imitate(search, source);
		imitate(result, source);
		
		Mat sourceMat = getMat(source);
		Mat searchMat = getMat(search);
		Mat& resultMat = result.getMat(); // ideally FloatImage is ofImage and we won't need this
		matchRegion(sourceMat, region, searchMat, resultMat);
	}
	
	void matchRegion(Mat& source, ofRectangle& region, Mat& search, Mat& result) {
		Mat sourceMat = Mat(source, makeRect(region));
		
		matchTemplate(search, sourceMat, result, CV_TM_CCOEFF_NORMED);
	}
	
	ofPoint matchRegion(ofImage& source, ofRectangle& region, ofImage& search) {
		FloatImage result;
		
		imitate(result, source);
	}
	
	float weightedAverageAngle(const vector<Vec4i>& lines) {
		float angleSum = 0;
		ofVec2f start, end;
		float weights = 0;
		for(int i = 0; i < lines.size(); i++) {
			start.set(lines[i][0], lines[i][1]);
			end.set(lines[i][2], lines[i][3]);
			ofVec2f diff = end - start;
			float length = diff.length();
			float weight = length * length;
			float angle = atan2f(diff.y, diff.x);
			angleSum += angle * weight;
			weights += weight;
		}
		return angleSum / weights;
	}
	
	void autorotate(ofImage& original, ofImage& thresh, ofImage& output, float* rotation) {
		imitate(output, original);
		
		Mat originalMat = getMat(original);
		Mat threshMat = getMat(thresh);
		Mat outputMat = getMat(output);
		vector<Vec4i> lines;
		
		double distanceResolution = 1;
		double angleResolution = CV_PI / 180;
		int voteThreshold = 10;
		double minLineLength = (originalMat.rows + originalMat.cols) / 8;
		double maxLineGap = 3;
		HoughLinesP(threshMat, lines, distanceResolution, angleResolution, voteThreshold, minLineLength, maxLineGap);
		
		float rotationAmount = ofRadToDeg(weightedAverageAngle(lines));
		
		rotate(original, output, rotationAmount);
		
		if(rotation != NULL) {
			*rotation = rotationAmount;
		}
	}
	
	// only works with single channel kernels
	void convolve(ofImage& source, FloatImage& kernel, ofImage& destination) {
		imitate(destination, source);
		
		Mat sourceMat = getMat(source);
		Mat kernelMat = kernel.getMat();
		Mat destinationMat = getMat(destination);
		
		flip(kernelMat, kernelMat, -1); // flip both axes
		// we may need to 'set the anchor position'...?
		// http://opencv.willowgarage.com/documentation/cpp/imgproc_image_filtering.html?highlight=filter2d#filter2D
		
		if(sourceMat.channels() == 1) {
			filter2D(sourceMat, destinationMat, -1, kernelMat);
		} else {
			vector<Mat> mvSource, mvDestination;
			split(sourceMat, mvSource);
			split(destinationMat, mvDestination);
			for(int i = 0; i < mvSource.size(); i++) {
				filter2D(mvSource[i], mvDestination[i], -1, kernelMat);
			}
			merge(mvDestination, destinationMat);
		}
		
		flip(kernelMat, kernelMat, -1); // flip the kernel back
	}
	
	void convolve(ofImage& img, FloatImage& kernel) {
		convolve(img, kernel, img);
	}
	
	ofVec2f findMaxLocation(FloatImage& img) {
		return findMaxLocation(img.getMat());
	}
	
	ofVec2f findMaxLocation(Mat& mat) {
		double minVal, maxVal;
		cv::Point minLoc, maxLoc;
		minMaxLoc(mat, &minVal, &maxVal, &minLoc, &maxLoc);
		return ofVec2f(maxLoc.x, maxLoc.y);
	}
	
	void getBoundingBox(ofImage& img, ofRectangle& box, int thresh, bool invert) {
		Mat mat = getMat(img);
		int flags = (invert ? THRESH_BINARY_INV : THRESH_BINARY);
		
		Mat rowMat = meanRows(mat);
		threshold(rowMat, rowMat, thresh, 255, flags);
		box.y = findFirst(rowMat, 255);
		box.height = findLast(rowMat, 255);
		box.height -= box.y;
		
		Mat colMat = meanCols(mat);
		threshold(colMat, colMat, thresh, 255, flags);
		box.x = findFirst(colMat, 255);
		box.width = findLast(colMat, 255);
		box.width -= box.x;
	}
	
	int forceOdd(int x) {
		return (x / 2) * 2 + 1;
	}
	
	void blur(FloatImage& original, FloatImage& blurred, int size) {
		size = forceOdd(size);
		imitate(blurred, original);
		GaussianBlur(original.getMat(), blurred.getMat(), cv::Size(size, size), 0, 0);
	}
	
	void medianBlur(ofImage& img, int size) {
		size = forceOdd(size);
		Mat mat = getMat(img);
		medianBlur(mat, mat, size);
	}
	
	void resize(ofImage& source, ofImage& destination, int interpolation) {
		Mat sourceMat = getMat(source);
		Mat destinationMat = getMat(destination);
		resize(sourceMat, destinationMat, destinationMat.size(), 0, 0, interpolation);
	}
	
	void resize(ofImage& source, ofImage& destination, float xScale, float yScale, int interpolation) {
		ofImageType sourceType = source.getPixelsRef().getImageType();
		destination.allocate(source.getWidth() * xScale, source.getHeight() * yScale, sourceType);
		resize(source, destination, interpolation);
	}
	
	int findFirst(const Mat& arr, unsigned char target) {
		for(int i = 0; i < arr.rows; i++) {
			if(arr.at<unsigned char>(i) == target) {
				return i;
			}
		}
		return 0;
	}
	
	int findLast(const Mat& arr, unsigned char target) {
		for(int i = arr.rows - 1; i >= 0; i--) {
			if(arr.at<unsigned char>(i) == target) {
				return i;
			}
		}
		return 0;
	}
	
	Mat meanCols(const Mat& mat) {
		Mat colMat(mat.cols, 1, mat.type());
		for(int i = 0; i < mat.cols; i++) {
			colMat.row(i) = mean(mat.col(i));
		}	
		return colMat;
	}
	
	Mat meanRows(const Mat& mat) {
		Mat rowMat(mat.rows, 1, mat.type());
		for(int i = 0; i < mat.rows; i++) {
			rowMat.row(i) = mean(mat.row(i));
		}
		return rowMat;
	}
	
	Mat sumCols(const Mat& mat) {
		Mat colMat(mat.cols, 1, CV_32FC1);
		for(int i = 0; i < mat.cols; i++) {
			colMat.row(i) = sum(mat.col(i));
		}	
		return colMat;
	}
	
	Mat sumRows(const Mat& mat) {
		Mat rowMat(mat.rows, 1, CV_32FC1);
		for(int i = 0; i < mat.rows; i++) {
			rowMat.row(i) = sum(mat.row(i));
		}
		return rowMat;
	}
	
}