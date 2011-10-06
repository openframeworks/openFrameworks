#include "ofxCv/Wrappers.h"

namespace ofxCv {
	
	using namespace cv;
	
	Vec3b convertColor(Vec3b color, int code) {
		Mat_<Vec3b> mat(1, 1, CV_8UC3);
		mat(0, 0) = color;
		cvtColor(mat, mat, code);
		return mat(0, 0);
	}
	
	ofColor convertColor(ofColor color, int code) {
		Vec3b cvColor(color.r, color.g, color.b);
		Vec3b result = convertColor(cvColor, code);
		return ofColor(result[0], result[1], result[2], color.a);
	}	
	
	ofPolyline convexHull(ofPolyline& polyline) {
		vector<cv::Point2f> contour = toCv(polyline);
		vector<cv::Point2f> hull;
		convexHull(Mat(contour), hull);
		return toOf(hull);
	}
	
	cv::RotatedRect minAreaRect(ofPolyline& polyline) {
		return minAreaRect(Mat(toCv(polyline)));
	}
	
	cv::RotatedRect fitEllipse(ofPolyline& polyline) {
		return fitEllipse(Mat(toCv(polyline)));
	}	
	
	void invert(ofImage& img) {
		Mat imgMat = toCv(img);
		bitwise_not(imgMat, imgMat);
	}
	
	// if you're doing the same rotation multiple times,
	// it's better to precompute the displacement and use remap
	// fill should be an ofColor, and we need a function to convert ofColor to cv::Scalar
	void rotate(ofImage& original, ofImage& rotated, double angle, unsigned char fill, int interpolation) {
		imitate(rotated, original);
		
		Mat originalMat = toCv(original);
		Mat rotatedMat = toCv(rotated);
		
		Point2f center(originalMat.rows / 2, originalMat.cols / 2);
		Mat rotationMatrix = getRotationMatrix2D(center, angle, 1);
		warpAffine(originalMat, rotatedMat, rotationMatrix, originalMat.size(), interpolation, BORDER_CONSTANT, Scalar(fill));
	}
	
	void autothreshold(ofImage& original, ofImage& thresh, bool invert) {
		imitate(thresh, original);
		
		Mat originalMat = toCv(original);
		Mat threshMat = toCv(thresh);
		
		// this might only work on grayscale atm...
		// THRESH_OTSU takes 4x as long as normal thresholding
		int flags = THRESH_OTSU | (invert ? THRESH_BINARY_INV : THRESH_BINARY);
		threshold(originalMat, threshMat, 0, 255, flags);
	}
	
	void autothreshold(ofImage& img, bool invert) {
		autothreshold(img, img, invert);
	}
	/*
	 void threshold(FloatImage& img, float value, bool invert) {
	 threshold(img, img, value, invert);
	 }
	 
	 void threshold(FloatImage& original, FloatImage& thresh, float value, bool invert) {
	 threshold(original.toCv(), thresh.toCv(), value, 1, invert ? THRESH_BINARY_INV : THRESH_BINARY);
	 }
	 
	 void matchRegion(ofImage& source, ofRectangle& region, ofImage& search, FloatImage& result) {
	 imitate(search, source);
	 imitate(result, source);
	 
	 Mat sourceMat = toCv(source);
	 Mat searchMat = toCv(search);
	 Mat& resultMat = result.toCv(); // ideally FloatImage is ofImage and we won't need this
	 matchRegion(sourceMat, region, searchMat, resultMat);
	 }
	 
	 void matchRegion(Mat& source, ofRectangle& region, Mat& search, Mat& result) {
	 Mat sourceMat = Mat(source, toCv(region));
	 
	 matchTemplate(search, sourceMat, result, CV_TM_CCOEFF_NORMED);
	 }
	 */
	
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
		
		Mat originalMat = toCv(original);
		Mat threshMat = toCv(thresh);
		Mat outputMat = toCv(output);
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
	
	/*
	 // only works with single channel kernels
	 void convolve(ofImage& source, FloatImage& kernel, ofImage& destination) {
	 imitate(destination, source);
	 
	 Mat sourceMat = toCv(source);
	 Mat kernelMat = kernel.toCv();
	 Mat destinationMat = toCv(destination);
	 
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
	 return findMaxLocation(img.toCv());
	 }
	 */
	 
	void resize(ofImage& source, ofImage& destination, int interpolation) {
		Mat sourceMat = toCv(source);
		Mat destinationMat = toCv(destination);
		resize(sourceMat, destinationMat, destinationMat.size(), 0, 0, interpolation);
	}
	
	void resize(ofImage& source, ofImage& destination, float xScale, float yScale, int interpolation) {
		ofImageType sourceType = source.getPixelsRef().getImageType();
		destination.allocate(source.getWidth() * xScale, source.getHeight() * yScale, sourceType);
		resize(source, destination, interpolation);
	}
}