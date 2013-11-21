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
 */
 
#pragma once

#include "ofxCv.h"

namespace ofxCv {

	using namespace cv;
	
	class Intrinsics {
	public:
		// kinect is 6.66mm(H) x 5.32mm(V)
		void setup(Mat cameraMatrix, cv::Size imageSize, cv::Size sensorSize = cv::Size(0, 0));
		void setImageSize(cv::Size imgSize);
		Mat getCameraMatrix() const;
		cv::Size getImageSize() const;
		cv::Size getSensorSize() const;
		cv::Point2d getFov() const;
		double getFocalLength() const;
		double getAspectRatio() const;
		Point2d getPrincipalPoint() const;
		void loadProjectionMatrix(float nearDist = 10., float farDist = 10000., cv::Point2d viewportOffset = cv::Point2d(0, 0)) const;
	protected:
		Mat cameraMatrix;
		cv::Size imageSize, sensorSize;
		cv::Point2d fov;
		double focalLength, aspectRatio;
		Point2d principalPoint;
	};
	
	enum CalibrationPattern {CHESSBOARD, CIRCLES_GRID, ASYMMETRIC_CIRCLES_GRID};
	
	class Calibration : public ofNode {
	public:
		Calibration();
		
		void save(string filename, bool absolute = false) const;
		void load(string filename, bool absolute = false);
		
		void setPatternType(CalibrationPattern patternType);
		void setPatternSize(int xCount, int yCount);
		void setSquareSize(float squareSize);
		/// set this to the pixel size of your smallest square. default is 11
		void setSubpixelSize(int subpixelSize);

		bool add(Mat img);
		bool clean(float minReprojectionError = 2.f);
		bool calibrate();
		bool calibrateFromDirectory(string directory);
		bool findBoard(Mat img, vector<Point2f> &pointBuf, bool refine = true);
		void setIntrinsics(Intrinsics& distortedIntrinsics, Mat& distortionCoefficients);
        
		void undistort(Mat img, int interpolationMode = INTER_NEAREST);
		void undistort(Mat src, Mat dst, int interpolationMode = INTER_NEAREST);
		
		ofVec2f undistort(ofVec2f& src) const;
		void undistort(vector<ofVec2f>& src, vector<ofVec2f>& dst) const;
		
		bool getTransformation(Calibration& dst, Mat& rotation, Mat& translation);
		
		float getReprojectionError() const;
		float getReprojectionError(int i) const;
		
		const Intrinsics& getDistortedIntrinsics() const;
		const Intrinsics& getUndistortedIntrinsics() const;
		Mat getDistCoeffs() const;
		
		// if you want a wider fov, say setFillFrame(false) before load() or calibrate()
		void setFillFrame(bool fillFrame);
		
		int size() const;
		cv::Size getPatternSize() const;
		float getSquareSize() const;
		static vector<Point3f> createObjectPoints(cv::Size patternSize, float squareSize, CalibrationPattern patternType);
		
		void customDraw();
		void draw(int i) const;
		void draw3d() const;
		void draw3d(int i) const;
		
		bool isReady();
		vector<vector<Point2f> > imagePoints;
		
	protected:
		CalibrationPattern patternType;
		cv::Size patternSize, addedImageSize, subpixelSize;
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
		
		bool ready;
	};
	
}