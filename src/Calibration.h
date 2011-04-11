#pragma once

/*
	this class handles per-camera intrinsic calibration and undistortion.
	given a series of chessboard images, it will calculate the intrinsics.
	to use it:
	
	0 either load() from a yml file (skip to 5),
	or do the calibration as follows
	1 set the board and physical square size of the chess board. whatever
	if your squares are in mm, your focal length will also be in mm.
	2 add() each image containing a chess board
	3 when all the images are added, call calibrate()
	4 now you can save() a yml calibration file 
	5 now you can undistort() incoming images. 
	
	to do inter-camera (extrinsics) calibration, you need to first calibrate
	each camera individually. then use getTransformation to determine the
	rotation and translation from camera to another.
	
	- - --
	
	need to clena up getters by adding another class
	will help distinguishing distorted from undistorted parameters
*/

#include "ofxCv.h"

namespace ofxCv {
	
	using namespace cv;
	
	class Intrinsics {
	public:
		void setup(Mat cameraMatrixm cv::Size imageSize, cv::Size sensorSize);
		Mat getCameraMatrix() const;
		cv::Size getImageSize() const;
		cv::Size getSensorSize() const;
		cv::Point2d getFov() const;
		double getFocalLength() const;
		double getAspectRatio() const;
		Point2d getPrincipalPoint() const;
	protected:
		Mat cameraMatrix;
		cv::Size imageSize, sensorSize;
		cv::Point2d fov;
		double focalLength, aspectRatio;
		Point2d principalPoint;
	};
	
	class Calibration {
	public:	
		Calibration();
				
		void save(string filename, bool absolute = false) const;
		void load(string filename, bool absolute = false);
		
		void setBoardSize(int xCount, int yCount);
		void setSquareSize(float squareSize);
		bool add(ofImage& img);
		bool calibrate();
		bool calibrateFromDirectory(string directory);
		void undistort(ofImage& img);
		
		void getTransformation(Calibration& dst, Mat& rotation, Mat& translation);
		
		float getReprojectionError() const;
		float getReprojectionError(int i) const;
		
		const Intrinsics& getDistortedIntrinsics() const;
		const Intrinsics& getUndistortedIntrinsics() const;
		
		// if you want a wider fov, say setFillFrame(false) before load() or calibrate()
		void setFillFrame(bool fillFrame);
		
		int size() const;
		
		void draw() const;
		void draw(int i) const;
		void draw3d() const;
		void draw3d(int i) const;
		
	protected:
		vector<vector<Point2f> > imagePoints;
		cv::Size boardSize, imageSize;
		float squareSize;
		Mat grayMat;
		
		Mat distCoeffs;
		
		vector<Mat> boardRotations, boardTranslations;
		vector<vector<Point3f> > objectPoints;
		
		float reprojectionError;
		vector<float> perViewErrors;
		
		bool fillFrame;
		Mat undistortBuffer;
		Mat undistortMapX, undistortMapY;
		
		void updateObjectPoints();
		void updateReprojectionError();
		void updateUndistortion();
		
		Intrinsics distortedIntrinsics;
		Intrinsics undistortedIntrinsics;
	};
	
}