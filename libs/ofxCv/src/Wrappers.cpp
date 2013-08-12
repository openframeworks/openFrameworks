#include "ofxCv/Wrappers.h"

namespace ofxCv {
	
	using namespace cv;
	
	void loadMat(Mat& mat, string filename) {
		FileStorage fs(ofToDataPath(filename), FileStorage::READ);
		fs["Mat"] >> mat;
	}
	
	void saveMat(Mat mat, string filename) {
		FileStorage fs(ofToDataPath(filename), FileStorage::WRITE);
		fs << "Mat" << mat;
	}
	
	void saveImage(Mat& mat, string filename) {
		if(mat.depth() == CV_8U) {
			ofPixels pix8u;
			toOf(mat, pix8u);
			ofSaveImage(pix8u, filename);
		} else if(mat.depth() == CV_16U) {
			ofShortPixels pix16u;
			toOf(mat, pix16u);
			ofSaveImage(pix16u, filename);
		} else if(mat.depth() == CV_32F) {
			ofFloatPixels pix32f;
			toOf(mat, pix32f);
			ofSaveImage(pix32f, filename);
		}
	}
	
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
	
	// this should be replaced by c++ 2.0 api style code once available
	vector<cv::Vec4i> convexityDefects(const vector<cv::Point>& contour) {
		vector<int> hullIndices;
		convexHull(Mat(contour), hullIndices, false, false);
		vector<cv::Vec4i> convexityDefects;
		if(hullIndices.size() > 0 && contour.size() > 0) {		
			CvMat contourMat = cvMat(1, contour.size(), CV_32SC2, (void*) &contour[0]);
			CvMat hullMat = cvMat(1, hullIndices.size(), CV_32SC1, (void*) &hullIndices[0]);
			CvMemStorage* storage = cvCreateMemStorage(0);
			CvSeq* defects = cvConvexityDefects(&contourMat, &hullMat, storage);
			for(int i = 0; i < defects->total; i++){
				CvConvexityDefect* cur = (CvConvexityDefect*) cvGetSeqElem(defects, i);
				cv::Vec4i defect;
				defect[0] = cur->depth_point->x;
				defect[1] = cur->depth_point->y;
				defect[2] = (cur->start->x + cur->end->x) / 2;
				defect[3] = (cur->start->y + cur->end->y) / 2;
				convexityDefects.push_back(defect);
			}
			cvReleaseMemStorage(&storage);
		}
		return convexityDefects;
	}
	
	vector<cv::Vec4i> convexityDefects(const ofPolyline& polyline) {
		vector<cv::Point2f> contour2f = toCv(polyline);
		vector<cv::Point2i> contour2i;
		Mat(contour2f).copyTo(contour2i);
		return convexityDefects(contour2i);
	}
	
	cv::RotatedRect minAreaRect(const ofPolyline& polyline) {
		return minAreaRect(Mat(toCv(polyline)));
	}
	
	cv::RotatedRect fitEllipse(const ofPolyline& polyline) {
		return fitEllipse(Mat(toCv(polyline)));
	}
	
	void fitLine(const ofPolyline& polyline, ofVec2f& point, ofVec2f& direction) {
		Vec4f line;
		fitLine(Mat(toCv(polyline)), line, CV_DIST_L2, 0, .01, .01);
		direction.set(line[0], line[1]);
		point.set(line[2], line[3]);
	}
    
	ofMatrix4x4 estimateAffine3D(vector<ofVec3f>& from, vector<ofVec3f>& to, float accuracy) {
		if(from.size() != to.size() || from.size() == 0 || to.size() == 0) {
			return ofMatrix4x4();
		}
		vector<unsigned char> outliers;
		return estimateAffine3D(from, to, outliers, accuracy);
	}
    
	ofMatrix4x4 estimateAffine3D(vector<ofVec3f>& from, vector<ofVec3f>& to, vector<unsigned char>& outliers, float accuracy) {
		Mat fromMat(1, from.size(), CV_32FC3, &from[0]);
		Mat toMat(1, to.size(), CV_32FC3, &to[0]);
		Mat affine;
		estimateAffine3D(fromMat, toMat, affine, outliers, 3, accuracy);
		ofMatrix4x4 affine4x4;
		affine4x4.set(affine.ptr<double>());
		affine4x4(3, 0) = 0;
		affine4x4(3, 1) = 0;
		affine4x4(3, 2) = 0;
		affine4x4(3, 3) = 1;
		Mat affine4x4Mat(4, 4, CV_32F, affine4x4.getPtr());
		affine4x4Mat = affine4x4Mat.t();
		affine4x4.set(affine4x4Mat.ptr<float>());
		return affine4x4;
	} 
}