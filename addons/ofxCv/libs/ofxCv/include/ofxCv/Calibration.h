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
#include "ofNode.h"

namespace ofxCv {
	class Intrinsics {
	public:
        void setup(float focalLengthMm, cv::Size imageSizePx, cv::Size2f sensorSizeMm, cv::Point2d principalPointPct = cv::Point2d(.5,.5));
		void setup(cv::Mat cameraMatrix, cv::Size imageSizePx, cv::Size2f sensorSizeMm = cv::Size2f(0, 0));
		void setImageSize(cv::Size imgSize);
		cv::Mat getCameraMatrix() const;
		cv::Size getImageSize() const;
		cv::Size2f getSensorSize() const;
		cv::Point2d getFov() const;
		double getFocalLength() const;
		double getAspectRatio() const;
		cv::Point2d getPrincipalPoint() const;
		void loadProjectionMatrix(float nearDist = 10., float farDist = 10000., cv::Point2d viewportOffset = cv::Point2d(0, 0)) const;
	protected:
        void updateValues();
		cv::Mat cameraMatrix;
        cv::Size imageSize;
        cv::Size2f sensorSize;
		cv::Point2d fov;
		double focalLength, aspectRatio;
		cv::Point2d principalPoint;
	};
	
	enum CalibrationPattern {CHESSBOARD, CIRCLES_GRID, ASYMMETRIC_CIRCLES_GRID};
	
	class Calibration : public ofNode {
	public:
		Calibration();
		
		void save(std::string filename, bool absolute = false) const;
		void load(std::string filename, bool absolute = false);
		void reset();

		void setPatternType(CalibrationPattern patternType);
		void setPatternSize(int xCount, int yCount);
		void setSquareSize(float squareSize);
		/// set this to the pixel size of your smallest square. default is 11
		void setSubpixelSize(int subpixelSize);

		bool add(cv::Mat img);
		bool clean(float minReprojectionError = 2.f);
		bool calibrate();
		bool calibrateFromDirectory(std::string directory);
		bool findBoard(cv::Mat img, std::vector<cv::Point2f> &pointBuf, bool refine = true);
		void setIntrinsics(Intrinsics& distortedIntrinsics);
        void setDistortionCoefficients(float k1, float k2, float p1, float p2, float k3=0, float k4=0, float k5=0, float k6=0);
        
		void undistort(cv::Mat img, int interpolationMode = cv::INTER_NEAREST);
		void undistort(cv::Mat src, cv::Mat dst, int interpolationMode = cv::INTER_NEAREST);
		
		ofVec2f undistort(ofVec2f& src) const;
		void undistort(std::vector<ofVec2f>& src, std::vector<ofVec2f>& dst) const;
		
		bool getTransformation(Calibration& dst, cv::Mat& rotation, cv::Mat& translation);
		
		float getReprojectionError() const;
		float getReprojectionError(int i) const;
		
		const Intrinsics& getDistortedIntrinsics() const;
		const Intrinsics& getUndistortedIntrinsics() const;
		cv::Mat getDistCoeffs() const;
		
		// if you want a wider fov, say setFillFrame(false) before load() or calibrate()
		void setFillFrame(bool fillFrame);
		
		int size() const;
		cv::Size getPatternSize() const;
		float getSquareSize() const;
		static std::vector<cv::Point3f> createObjectPoints(cv::Size patternSize, float squareSize, CalibrationPattern patternType);
		
		void customDraw();
		void draw(int i) const;
		void draw3d() const;
		void draw3d(int i) const;
		
		bool isReady();
		std::vector<std::vector<cv::Point2f> > imagePoints;
		
	protected:
		CalibrationPattern patternType;
		cv::Size patternSize, addedImageSize, subpixelSize;
		float squareSize;
		cv::Mat grayMat;
		
		cv::Mat distCoeffs;
		
		std::vector<cv::Mat> boardRotations, boardTranslations;
		std::vector<std::vector<cv::Point3f> > objectPoints;
		
		float reprojectionError;
		std::vector<float> perViewErrors;
		
		bool fillFrame;
		cv::Mat undistortBuffer;
		cv::Mat undistortMapX, undistortMapY;
		
		void updateObjectPoints();
		void updateReprojectionError();
		void updateUndistortion();
		
		Intrinsics distortedIntrinsics;
		Intrinsics undistortedIntrinsics;
		
		bool ready;
	};
	
}
