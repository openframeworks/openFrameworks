#include "ofxCv.h"

#include "ofMath.h"

namespace ofxCv {
	
	using namespace cv;
	
	Mat toCv(Mat& mat) {
		return mat;
	}
	
	Mat toCv(ofBaseHasPixels& img) {
		return toCv(img.getPixelsRef());
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
	
	vector<cv::Point2f> toCv(const ofPolyline& polyline) {
		vector<cv::Point2f> contour(polyline.size());
		for(int i = 0; i < polyline.size(); i++) {
			contour[i].x = polyline[i].x;
			contour[i].y = polyline[i].y;
		}
		return contour;		
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
	
	ofPolyline toOf(cv::RotatedRect& rect) {
		vector<cv::Point2f> corners(4);
		rect.points(&corners[0]);
		ofPolyline polyline = toOf(corners);
		return polyline;
	}
	
	void loadImage(Mat& mat, string filename) {
		mat = imread(ofToDataPath(filename));
	}
	
	void saveImage(Mat mat, string filename) {
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
	
	float getMaxVal(int depth) {
		switch(depth) {
			case CV_8U: return numeric_limits<uint8_t>::max();
			case CV_16U: return numeric_limits<uint16_t>::max();
				
			case CV_8S: return numeric_limits<int8_t>::max();
			case CV_16S: return numeric_limits<int16_t>::max();
			case CV_32S: return numeric_limits<int32_t>::max();
				
			case CV_32F: return 1;
			case CV_64F: return 1;
		}
	}
	
	float getMaxVal(const Mat& mat) {
		return getMaxVal(mat.depth());
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
	
	/*
	 void blur(FloatImage& original, FloatImage& blurred, int size) {
	 size = forceOdd(size);
	 imitate(blurred, original);
	 GaussianBlur(original.toCv(), blurred.toCv(), cv::Size(size, size), 0, 0);
	 }
	 */
	
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
	
	void drawHighlightString(string text, int x, int y, ofColor background, ofColor foreground) {
		int textWidth =  10 + text.length() * 8;
		ofSetColor(background);
		ofFill();
		ofRect(x - 5, y - 12, textWidth, 20);
		ofSetColor(foreground);
		ofDrawBitmapString(text, x, y);
	}
	
	// for some reason, cvtColor handles this info internally rather than having
	// a single helper function. so we have to create a helper function to aid
	// in doing the allocationg ofxCv::convertColor()
#define mkcase(x, y) {case x: return y;}
	int getTargetChannelsFromCode(int conversionCode) {
		switch(conversionCode) {
				mkcase(CV_RGB2RGBA,4)	mkcase(CV_RGBA2RGB,3) mkcase(CV_RGB2BGRA,4)
				mkcase(CV_RGBA2BGR,3) mkcase(CV_BGR2RGB,3) mkcase(CV_BGRA2RGBA,4)
				mkcase(CV_BGR2GRAY,1) mkcase(CV_RGB2GRAY,1) mkcase(CV_GRAY2RGB,3)
				mkcase(CV_GRAY2RGBA,4) mkcase(CV_BGRA2GRAY,1) mkcase(CV_RGBA2GRAY,1)
				mkcase(CV_BGR5652BGR,3) mkcase(CV_BGR5652RGB,3) mkcase(CV_BGR5652BGRA,4)
				mkcase(CV_BGR5652RGBA,4) mkcase(CV_BGR5652GRAY,1) mkcase(CV_BGR5552BGR,3)
				mkcase(CV_BGR5552RGB,3) mkcase(CV_BGR5552BGRA,4) mkcase(CV_BGR5552RGBA,4)
				mkcase(CV_BGR5552GRAY,1) mkcase(CV_BGR2XYZ,3) mkcase(CV_RGB2XYZ,3)
				mkcase(CV_XYZ2BGR,3) mkcase(CV_XYZ2RGB,3) mkcase(CV_BGR2YCrCb,3)
				mkcase(CV_RGB2YCrCb,3) mkcase(CV_YCrCb2BGR,3) mkcase(CV_YCrCb2RGB,3)
				mkcase(CV_BGR2HSV,3) mkcase(CV_RGB2HSV,3) mkcase(CV_BGR2Lab,3)
				mkcase(CV_RGB2Lab,3) mkcase(CV_BayerGB2BGR,3) mkcase(CV_BayerBG2RGB,3)
				mkcase(CV_BayerGB2RGB,3) mkcase(CV_BayerRG2RGB,3) mkcase(CV_BGR2Luv,3)
				mkcase(CV_RGB2Luv,3) mkcase(CV_BGR2HLS,3) mkcase(CV_RGB2HLS,3)
				mkcase(CV_HSV2BGR,3) mkcase(CV_HSV2RGB,3) mkcase(CV_Lab2BGR,3)
				mkcase(CV_Lab2RGB,3) mkcase(CV_Luv2BGR,3) mkcase(CV_Luv2RGB,3)
				mkcase(CV_HLS2BGR,3) mkcase(CV_HLS2RGB,3) mkcase(CV_BayerBG2RGB_VNG,3)
				mkcase(CV_BayerGB2RGB_VNG,3) mkcase(CV_BayerRG2RGB_VNG,3)
				mkcase(CV_BayerGR2RGB_VNG,3) mkcase(CV_BGR2HSV_FULL,3)
				mkcase(CV_RGB2HSV_FULL,3) mkcase(CV_BGR2HLS_FULL,3)
				mkcase(CV_RGB2HLS_FULL,3) mkcase(CV_HSV2BGR_FULL,3)
				mkcase(CV_HSV2RGB_FULL,3) mkcase(CV_HLS2BGR_FULL,3)
				mkcase(CV_HLS2RGB_FULL,3) mkcase(CV_LBGR2Lab,3) mkcase(CV_LRGB2Lab,3)
				mkcase(CV_LBGR2Luv,3) mkcase(CV_LRGB2Luv,3) mkcase(CV_Lab2LBGR,4)
				mkcase(CV_Lab2LRGB,4) mkcase(CV_Luv2LBGR,4) mkcase(CV_Luv2LRGB,4)
				mkcase(CV_BGR2YUV,3) mkcase(CV_RGB2YUV,3) mkcase(CV_YUV2BGR,3)
				mkcase(CV_YUV2RGB,3)
			default: return 0;
		}
	}
}