#include "Calibration.h"

#include "ofDirectoryLister.h"

namespace ofxCv {
	using namespace cv;
	
	Calibration::Calibration() :
	boardSize(cv::Size(9, 6)),
	squareSize(1),
	fillFrame(true) {
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
	void Calibration::setFillFrame(bool fillFrame) {
		this->fillFrame = fillFrame;
	}
	bool Calibration::add(ofImage& img) {
		imageSize = cv::Size(img.getWidth(), img.getHeight());
		
		vector<Point2f> pointBuf;
		Mat imgMat = getMat(img);
		
		// find corners
		// no CV_CALIB_CB_FAST_CHECK, it breaks on dark images (e.g., dark IR images from kinect)
		int chessFlags = CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE;
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
		} else {		
			ofLog(OF_LOG_ERROR, "Calibration::add() failed, maybe your boardSize is wrong or the image has poor lighting?");
		}
		return found;
	}
	bool Calibration::calibrate() {
		cameraMatrix = Mat::eye(3, 3, CV_64F);
    distCoeffs = Mat::zeros(8, 1, CV_64F);
    
		updateObjectPoints();
		
		bool calibFlags = 0; // uses more coeffs, but not necc. better
    float rms = calibrateCamera(objectPoints, imagePoints, imageSize, cameraMatrix, distCoeffs, boardRotations, boardTranslations, calibFlags);
    ofLog(OF_LOG_VERBOSE, "calibrateCamera() reports RMS error of " + ofToString(rms));
		
    bool ok = checkRange(cameraMatrix) && checkRange(distCoeffs);
    
		updateReprojectionError();
		updateUndistortion();
		
		return ok;
	}
	bool Calibration::calibrateFromDirectory(string directory) {
		ofDirectoryLister dirList;
		ofImage cur;
		dirList.listDir(directory);
		for(int i = 0; i < dirList.size(); i++) {
			cur.loadImage(dirList.getPath(i));
			if(!add(cur)) {
				ofLog(OF_LOG_ERROR, "Calibration::add() failed on " + dirList.getPath(i));
			}
		}
		return calibrate();
	}
	void Calibration::undistort(ofImage& img) {
		Mat imgMat = getMat(img);
		imgMat.copyTo(undistortBuffer);
		remap(undistortBuffer, imgMat, undistortMapX, undistortMapY, INTER_LINEAR);
	}
	void Calibration::getTransformation(Calibration& dst, Mat& rotation, Mat& translation) {
		if(imagePoints.size() == 0 || dst.imagePoints.size() == 0) {
			ofLog(OF_LOG_ERROR, "getTransformation() requires both Calibration objects to have just been calibrated");
			return;
		}
		if(imagePoints.size() != dst.imagePoints.size() || boardSize != dst.boardSize) {
			ofLog(OF_LOG_ERROR, "getTransformation() requires both Calibration objects to be trained simultaneously on the same board");
			return;
		}
		Mat fundamentalMatrix, essentionalMatrix;
		// uses CALIB_FIX_INTRINSIC by default
		stereoCalibrate(objectPoints,
										imagePoints, dst.imagePoints,
										cameraMatrix, distCoeffs,
										dst.cameraMatrix, dst.distCoeffs,
										imageSize, rotation, translation,
										essentionalMatrix, fundamentalMatrix);			
	}		
	float Calibration::getReprojectionError() const {
		return reprojectionError;
	}
	float Calibration::getReprojectionError(int i) const {
		return perViewErrors[i];
	}
	Mat Calibration::getDistortedCameraMatrix() {
		return cameraMatrix;
	}
	Mat Calibration::getUndistortedCameraMatrix() {
		return undistortedCameraMatrix;
	}
	Mat Calibration::getDistortionCoefficients() {
		return distCoeffs;
	}
	ofVec2f Calibration::getSensorSize() const {
		return ofVec2f(sensorWidth, sensorHeight);
	}
	ofVec2f Calibration::getDistortedFov() const {
		return ofVec2f(distortedFovx, distortedFovy);
	}
	ofVec2f Calibration::getUndistortedFov() const {
		return ofVec2f(fovx, fovy);
	}
	ofVec2f Calibration::getUndistortedPrincipalPoint() const {
		return ofVec2f(undistortedPrincipalPoint.x, undistortedPrincipalPoint.y);
	}
	ofVec2f Calibration::getImageSize() const {
		return ofVec2f(imageSize.width, imageSize.height);
	}
	float Calibration::getFocalLength() const {
		return focalLength;
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
			ofCircle(makeVec(imagePoints[i][j]), 5);
		}
		ofPopStyle();
	}
	void Calibration::draw3d() const {
		for(int i = 0; i < size(); i++) {
			draw3d(i);
		}
	}
	void Calibration::draw3d(int i) const {
		ofPushStyle();
		ofPushMatrix();
		ofNoFill();
		
		applyMatrix(makeMatrix(boardRotations[i], boardTranslations[i]));
		
		ofSetColor(ofColor::fromHsb(255 * i / size(), 255, 255));
		
		ofDrawBitmapString(ofToString(i), 0, 0);
		
		for(int j = 0; j < objectPoints[i].size(); j++) {
			ofPushMatrix();
			ofTranslate(makeVec(objectPoints[i][j]));
			ofCircle(0, 0, .5);
			ofPopMatrix();
		}
		glBegin(GL_LINE_STRIP);
		for(int j = 0; j < objectPoints[i].size(); j++) {
			ofVec3f cur = makeVec(objectPoints[i][j]);
			glVertex3f(cur.x, cur.y, cur.z);
		}
		glEnd();
		
		ofPopMatrix();
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
			projectPoints(Mat(objectPoints[i]), boardRotations[i], boardTranslations[i], cameraMatrix, distCoeffs, imagePoints2);
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
		undistortedCameraMatrix = getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, imageSize, fillFrame ? 0 : 1);
		initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(),
														undistortedCameraMatrix,
														imageSize, CV_16SC2, undistortMapX, undistortMapY);
		// this should be done with a small struct/class like CalibrationParameters
		calibrationMatrixValues(cameraMatrix, imageSize, sensorWidth, sensorHeight,
														distortedFovx, distortedFovy, focalLength, principalPoint, aspectRatio);
		calibrationMatrixValues(undistortedCameraMatrix, imageSize, sensorWidth, sensorHeight,
														fovx, fovy, focalLength, undistortedPrincipalPoint, aspectRatio);
	}
}	