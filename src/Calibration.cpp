#include "Calibration.h"

namespace ofxCv {
	using namespace cv;
	
	Calibration::Calibration() :
	boardSize(cv::Size(9, 6)),
	squareSize(1) {
	}
	void Calibration::save(string filename, bool absolute) const {
    FileStorage fs(ofToDataPath(filename, absolute), FileStorage::WRITE);
		fs << "image_width" << imageSize.width;
		fs << "image_height" << imageSize.height;
    fs << "camera_matrix" << cameraMatrix;
    fs << "distortion_coefficients" << distCoeffs;
		fs << "reprojection_error" << reprojectionError;
	}
	void Calibration::load(string filename, bool absolute) {
		FileStorage fs(ofToDataPath(filename, absolute), FileStorage::READ);
		imageSize.width = (int) fs["image_width"];
		imageSize.height = (int) fs["image_height"];
		fs["camera_matrix"] >> cameraMatrix;
		fs["distortion_coefficients"] >> distCoeffs;
		reprojectionError = (float) fs["reprojection_error"];
		updateUndistortion();
	}
	void Calibration::setBoardSize(int xCount, int yCount) {
		boardSize = cv::Size(xCount, yCount);
	}
	void Calibration::setSquareSize(float squareSize) {
		this->squareSize = squareSize;
	}
	void Calibration::add(ofImage& img) {
		imageSize = cv::Size(img.getWidth(), img.getHeight());
		
		vector<Point2f> pointBuf;
		Mat imgMat = getMat(img);
		
		// find corners
		int chessFlags = CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FAST_CHECK | CV_CALIB_CB_NORMALIZE_IMAGE;
		bool found = findChessboardCorners(imgMat, boardSize, pointBuf, chessFlags);
		
		// improve corner accuracy
		if(found) {
			if(imgMat.type() != CV_8UC1) {
				cvtColor(imgMat, grayMat, CV_RGB2GRAY);
			} else {
				grayMat = imgMat;
			}
			
			cornerSubPix(grayMat, pointBuf, cv::Size(11, 11),  cv::Size(-1,-1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1 ));
			
			imagePoints.push_back(pointBuf);
		}
	}
	bool Calibration::calibrate() {
		cameraMatrix = Mat::eye(3, 3, CV_64F);
    distCoeffs = Mat::zeros(8, 1, CV_64F);
    
		updateObjectPoints();
		
		bool calibFlags = 0; // uses more coeffs, but not necc. better
    float rms = calibrateCamera(objectPoints, imagePoints, imageSize, cameraMatrix, distCoeffs, rvecs, tvecs, calibFlags);
    ofLog(OF_LOG_VERBOSE, "calibrateCamera() reports RMS error of " + ofToString(rms));
		
    bool ok = checkRange(cameraMatrix) && checkRange(distCoeffs);
    
		updateReprojectionError();
		updateUndistortion();
		
		return ok;
	}
	void Calibration::undistort(ofImage& img) {
		Mat imgMat = getMat(img);
		imgMat.copyTo(undistortBuffer);
		remap(undistortBuffer, imgMat, undistortMapX, undistortMapY, INTER_LINEAR);
	}
	float Calibration::getReprojectionError() const {
		return reprojectionError;
	}
	float Calibration::getReprojectionError(int i) const {
		return perViewErrors[i];
	}
	Mat Calibration::getCameraMatrix() {
		return cameraMatrix;
	}
	Mat Calibration::getDistortionCoefficients() {
		return distCoeffs;
	}
	int Calibration::size() const {
		return imagePoints.size();
	}
	void Calibration::draw() const {
		for(int i = 0; i < size(); i++) {
			draw(i);
		}
	}
	void Calibration::draw(int i) const {
		ofPushStyle();
		ofNoFill();
		ofSetColor(ofColor::red);
		for(int j = 0; j < imagePoints[i].size(); j++) {
			ofCircle(makeVec2f(imagePoints[i][j]), 5);
		}
		ofPopStyle();
	}
	void Calibration::updateObjectPoints() {
		objectPoints.resize(1); // start with one vector<Point3f>
		objectPoints[0].resize(0); // make sure its 0 length
    for(int i = 0; i < boardSize.height; i++) {
			for(int j = 0; j < boardSize.width; j++) {
				// fill it up with a bunch of Point3fs for each grid point
				objectPoints[0].push_back(Point3f(float(j * squareSize), float(i * squareSize), 0));
			}
		}
		// then resize the vector<vector<Point3f> >, duplicating the first item
    objectPoints.resize(imagePoints.size(), objectPoints[0]);
	}
	void Calibration::updateReprojectionError() {
		vector<Point2f> imagePoints2;
		int totalPoints = 0;
		double totalErr = 0;
		
		perViewErrors.clear();
		perViewErrors.resize(objectPoints.size());
		
		for(int i = 0; i < objectPoints.size(); i++) {
			projectPoints(Mat(objectPoints[i]), rvecs[i], tvecs[i], cameraMatrix, distCoeffs, imagePoints2);
			double err = norm(Mat(imagePoints[i]), Mat(imagePoints2), CV_L2);
			int n = objectPoints[i].size();
			perViewErrors[i] = sqrt(err * err / n);
			totalErr += err * err;
			totalPoints += n;
			ofLog(OF_LOG_VERBOSE, "view " + ofToString(i) + " has error of " + ofToString(perViewErrors[i]));
		}
		
		reprojectionError = sqrt(totalErr / totalPoints);
		
		ofLog(OF_LOG_VERBOSE, "all views have error of " + ofToString(reprojectionError));
	}
	void Calibration::updateUndistortion() {
		initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(),
														getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, imageSize, 1, imageSize, 0),
														imageSize, CV_16SC2, undistortMapX, undistortMapY);
	}
}	