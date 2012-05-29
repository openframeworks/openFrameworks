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
	
	ofPolyline convexHull(const ofPolyline& polyline) {
		vector<cv::Point2f> contour = toCv(polyline);
		vector<cv::Point2f> hull;
		convexHull(Mat(contour), hull);
		return toOf(hull);
	}
	
	cv::RotatedRect minAreaRect(const ofPolyline& polyline) {
		return minAreaRect(Mat(toCv(polyline)));
	}
	
	cv::RotatedRect fitEllipse(const ofPolyline& polyline) {
		return fitEllipse(Mat(toCv(polyline)));
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
	
	void resize(ofImage& source, ofImage& destination, float xScale, float yScale, int interpolation) {
		ofImageType sourceType = source.getPixelsRef().getImageType();
		destination.allocate(source.getWidth() * xScale, source.getHeight() * yScale, sourceType);
		resize(source, destination, interpolation);
	}
}