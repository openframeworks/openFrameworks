#include "ofxCv.h"

#include "ofMath.h"

namespace ofxCv {
	
	using namespace cv;
	
	void imitate(ofPixels& mirror, ofPixels& original) {
		int mw = mirror.getWidth();
		int mh = mirror.getHeight();
		int ow = original.getWidth();
		int oh = original.getHeight();
		ofImageType mt = mirror.getImageType();
		ofImageType ot = original.getImageType();
		if(mw != ow || mh != oh || mt != ot) {
			mirror.allocate(ow, oh, ot);
		}
	}
	
	void imitate(ofImage& mirror, ofPixels& original) {
		int mw = mirror.getWidth();
		int mh = mirror.getHeight();
		int ow = original.getWidth();
		int oh = original.getHeight();
		ofImageType mt = mirror.getPixelsRef().getImageType();
		ofImageType ot = original.getImageType();
		if(mw != ow || mh != oh || mt != ot) {
			mirror.allocate(ow, oh, ot);
		}
	}
	
	void imitate(FloatImage& mirror, ofPixels& original) {
		int mw = mirror.getWidth();
		int mh = mirror.getHeight();
		int ow = original.getWidth();
		int oh = original.getHeight();
		if(mw != ow || mh != oh) {
			mirror.allocate(ow, oh);
		}
	}
	
	void imitate(FloatImage& mirror, FloatImage& original) {
		int mw = mirror.getWidth();
		int mh = mirror.getHeight();
		int ow = original.getWidth();
		int oh = original.getHeight();
		if(mw != ow || mh != oh) {
			mirror.allocate(ow, oh);
		}
	}
	
	int toCv(const ofImageType& ofType) {
		switch(ofType) {
			case OF_IMAGE_GRAYSCALE: return CV_8UC1;
			case OF_IMAGE_COLOR: return CV_8UC3;
			case OF_IMAGE_COLOR_ALPHA: return CV_8UC4;
			default: return CV_8UC1;
		}
	}
	
	void copy(Mat to, Mat from) {
		from.copyTo(to);
	}
	
	Mat toCv(ofBaseHasPixels& img) {
		return toCv(img.getPixelsRef());
	}
	
	Mat toCv(ofPixels& pix) {
		int cvImageType = toCv(pix.getImageType());
		return Mat(pix.getHeight(), pix.getWidth(), cvImageType, pix.getPixels(), 0);
	}
	
	Mat toCv(FloatImage& img) {
		return img.toCv();
	}
	
	Point2f toCv(ofVec2f& vec) {
		return Point2f(vec.x, vec.y);
	}
	
	Point3f toCv(ofVec3f& vec) {
		return Point3f(vec.x, vec.y, vec.z);
	}
	
	cv::Rect toCv(ofRectangle& rect) {
		return cv::Rect(rect.x, rect.y, rect.width, rect.height);
	}
	
	Mat toCv(ofMesh& mesh) {
		vector<ofVec3f>& vertices = mesh.getVertices();
		return Mat(1, vertices.size(), CV_32FC3, &vertices[0]);
	}
	
	ofVec2f toOf(Point2f point) {
		return ofVec2f(point.x, point.y);
	}
	
	ofVec3f toOf(Point3f point) {
		return ofVec3f(point.x, point.y, point.z);
	}
	
	ofRectangle toOf(cv::Rect rect) {
		return ofRectangle(rect.x, rect.y, rect.width, rect.height);
	}
	
	void loadImage(Mat& mat, string filename) {
		mat = imread(ofToDataPath(filename));
	}
	
	void saveImage(Mat& mat, string filename) {
		imwrite(ofToDataPath(filename), mat);
	}
	
	ofMatrix4x4 makeMatrix(Mat rotation, Mat translation) {
		Mat rot3x3;
		if(rotation.rows == 3 && rotation.cols == 3) {
			rot3x3 = rotation;
		} else {
			Rodrigues(rotation, rot3x3);
		}
		double* rm = rot3x3.ptr<double>(0);
		double* tm = translation.ptr<double>(0);
		return ofMatrix4x4(rm[0], rm[3], rm[6], 0.0f,
											 rm[1], rm[4], rm[7], 0.0f,
											 rm[2], rm[5], rm[8], 0.0f,
											 tm[0], tm[1], tm[2], 1.0f);
	}
	
	void drawMat(Mat& mat, float x, float y) {
		drawMat(mat, x, y, mat.cols, mat.rows);
	}
	
	void drawMat(Mat& mat, float x, float y, float width, float height) {
		int glType;
		Mat buffer;
		if(mat.depth() != CV_8U) {
			mat.convertTo(buffer, CV_8U);
		} else {
			buffer = mat;
		}
		if(mat.channels() == 1) {
			glType = GL_LUMINANCE;
		} else {
			glType = GL_RGB;
		}
		ofTexture tex;
		int w = buffer.cols;
		int h = buffer.rows;
		tex.allocate(w, h, glType);
		tex.loadData(buffer.ptr(), w, h, glType);
		tex.draw(x, y, width, height);
	}
	
	void applyMatrix(const ofMatrix4x4& matrix) {
		glMultMatrixf((GLfloat*) matrix.getPtr());
	}
	
	float getMaxVal(const Mat& mat) {
		switch(mat.depth()) {
			case CV_8U: return numeric_limits<uint8_t>::max();
			case CV_16U: return numeric_limits<uint16_t>::max();
				
			case CV_8S: return numeric_limits<int8_t>::max();
			case CV_16S: return numeric_limits<int16_t>::max();
			case CV_32S: return numeric_limits<int32_t>::max();
				
			case CV_32F: return 1;
			case CV_64F: return 1;
		}
	}
	
	void threshold(Mat src, Mat dst, float thresholdValue, bool invert) {
		int thresholdType = invert ? THRESH_BINARY_INV : THRESH_BINARY;
		float maxVal = getMaxVal(dst);
		cv::threshold(src, dst, thresholdValue, maxVal, thresholdType);
	}
	
	void threshold(Mat srcDst, float thresholdValue, bool invert) {
		threshold(srcDst, srcDst, thresholdValue, invert);
	}
	
	void convertColor(Mat src, Mat dst, int code) {
		cvtColor(src, dst, code);
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
	
	ofVec2f findMaxLocation(Mat& mat) {
		double minVal, maxVal;
		cv::Point minLoc, maxLoc;
		minMaxLoc(mat, &minVal, &maxVal, &minLoc, &maxLoc);
		return ofVec2f(maxLoc.x, maxLoc.y);
	}
	
	void getBoundingBox(ofImage& img, ofRectangle& box, int thresh, bool invert) {
		Mat mat = toCv(img);
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
		GaussianBlur(original.toCv(), blurred.toCv(), cv::Size(size, size), 0, 0);
	}
	
	void medianBlur(ofImage& img, int size) {
		size = forceOdd(size);
		Mat mat = toCv(img);
		medianBlur(mat, mat, size);
	}
	
	void warpPerspective(ofImage& src, ofImage& dst, Mat& m, int flags) {
		Mat srcMat = toCv(src);
		Mat dstMat = toCv(dst);
		warpPerspective(srcMat, dstMat, m, dstMat.size(), flags);
	}
	
	void warpPerspective(ofPixels& src, ofPixels& dst, Mat& m, int flags) {
		Mat srcMat = toCv(src);
		Mat dstMat = toCv(dst);
		warpPerspective(srcMat, dstMat, m, dstMat.size(), flags);
	}
	
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