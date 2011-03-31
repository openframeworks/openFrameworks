#pragma once

#include "ofxCv.h"

namespace ofxCv {
	
	using namespace cv;
	
	class Calibration {
	public:
		Calibration();
		
		void save(string filename, bool absolute = false) const;
		void load(string filename, bool absolute = false);
		
		// step 1: set the board and square size of the chess board
		void setBoardSize(int xCount, int yCount);
		void setSquareSize(float squareSize);
		
		// step 2: add each image containing a chess board
		void add(ofImage& img);
		
		// step 3: when you're done adding the images, do the calibration
		bool calibrate();
		
		// step 4: now you can use the calibration to undistort your image
		void undistort(ofImage& img);
		
		float getReprojectionError() const;
		float getReprojectionError(int i) const;
		Mat getCameraMatrix();
		Mat getDistortionCoefficients();
		
		int size() const;
		
		void draw() const;
		void draw(int i) const;
	protected:
		vector<vector<Point2f> > imagePoints;
		cv::Size boardSize, imageSize;
		float squareSize;
		Mat grayMat;
		Mat cameraMatrix, distCoeffs;
		
		vector<Mat> rvecs, tvecs;
		vector<vector<Point3f> > objectPoints;
		
		float reprojectionError;
		vector<float> perViewErrors;
		
		Mat undistortBuffer;
		Mat undistortMapX, undistortMapY;
		
		void updateObjectPoints();
		void updateReprojectionError();
		void updateUndistortion();
	};
	
}