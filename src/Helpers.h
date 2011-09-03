/*
 helpers offer new, commonly-needed functionality that is not present in OpenCv
 or openFrameworks.
*/

#pragma once

#include "opencv2/opencv.hpp"
#include "ofMain.h"

namespace ofxCv {

	using namespace cv;

	void loadImage(Mat& mat, string filename);
	void saveImage(Mat mat, string filename);
	void loadMat(Mat& mat, string filename);
	void saveMat(Mat mat, string filename);
	
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
	
	Mat minCols(const Mat& mat);
	Mat minRows(const Mat& mat);
	Mat maxCols(const Mat& mat);
	Mat maxRows(const Mat& mat);
	
	float weightedAverageAngle(const vector<Vec4i>& lines);
	
	static const ofColor cyanPrint = ofColor::fromHex(0x00abec);
	static const ofColor magentaPrint = ofColor::fromHex(0xec008c);
	static const ofColor yellowPrint = ofColor::fromHex(0xffee00);
	
	void drawHighlightString(string text, ofPoint position, ofColor background = ofColor::black, ofColor foreground = ofColor::white);
	void drawHighlightString(string text, int x, int y, ofColor background = ofColor::black, ofColor foreground = ofColor::white);
	
}