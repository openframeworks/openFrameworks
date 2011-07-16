#include "Calibration.h"

#include "ofFileUtils.h"

namespace ofxCv {
	using namespace cv;
	
	void Intrinsics::setup(Mat cameraMatrix, cv::Size imageSize, cv::Size sensorSize) {
		this->cameraMatrix = cameraMatrix;
		this->imageSize = imageSize;
		this->sensorSize = sensorSize;
		calibrationMatrixValues(cameraMatrix, imageSize, sensorSize.width, sensorSize.height,
														fov.x, fov.y, focalLength, principalPoint, aspectRatio);
	}
	
	Mat Intrinsics::getCameraMatrix() const {
		return cameraMatrix;
	}
	
	cv::Size Intrinsics::getImageSize() const {
		return imageSize;
	}
	
	cv::Size Intrinsics::getSensorSize() const {
		return sensorSize;
	}
	
	cv::Point2d Intrinsics::getFov() const {
		return fov;
	}
	
	double Intrinsics::getFocalLength() const {
		return focalLength;
	}
	
	double Intrinsics::getAspectRatio() const {
		return aspectRatio;
	}
	
	Point2d Intrinsics::getPrincipalPoint() const {
		return principalPoint;
	}
	
	Calibration::Calibration() :
	boardSize(cv::Size(10, 7)), squareSize(2.5), // based on Chessboard_A4.pdf, assuming world units are centimeters
	fillFrame(true),
	isReady(_isReady),
	_isReady(false) {
	}
	void Calibration::save(string filename, bool absolute) const {
    FileStorage fs(ofToDataPath(filename, absolute), FileStorage::WRITE);
		cv::Size imageSize = distortedIntrinsics.getImageSize();
		cv::Size sensorSize = distortedIntrinsics.getSensorSize();
		Mat cameraMatrix = distortedIntrinsics.getCameraMatrix();
		
    fs << "cameraMatrix" << cameraMatrix;
		fs << "imageSize_width" << imageSize.width;
		fs << "imageSize_height" << imageSize.height;
		fs << "sensorSize_width" << sensorSize.width;
		fs << "sensorSize_height" << sensorSize.height;
    fs << "distCoeffs" << distCoeffs;
		fs << "reprojectionError" << reprojectionError;
	}
	void Calibration::load(string filename, bool absolute) {
		FileStorage fs(ofToDataPath(filename, absolute), FileStorage::READ);
		cv::Size imageSize, sensorSize;
		Mat cameraMatrix;
		
		fs["cameraMatrix"] >> cameraMatrix;
		fs["imageSize_width"] >> imageSize.width;
		fs["imageSize_height"] >> imageSize.height;
		fs["sensorSize_width"] >> sensorSize.width;
		fs["sensorSize_height"] >> sensorSize.height;
		fs["distCoeffs"] >> distCoeffs;
		fs["reprojectionError"] >> reprojectionError;
		
		distortedIntrinsics.setup(cameraMatrix, imageSize, sensorSize);
		
		updateUndistortion();
		
		_isReady = true;
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
	bool Calibration::add(Mat img) {
		addedImageSize = img.size();
		
		vector<Point2f> pointBuf;
		
		// find corners
		// no CV_CALIB_CB_FAST_CHECK, because it breaks on dark images (e.g., dark IR images from kinect)
		int chessFlags = CV_CALIB_CB_ADAPTIVE_THRESH;// | CV_CALIB_CB_NORMALIZE_IMAGE;
		bool found = findChessboardCorners(img, boardSize, pointBuf, chessFlags);
		
		// improve corner accuracy
		if(found) {
			if(img.type() != CV_8UC1) {
				cvtColor(img, grayMat, CV_RGB2GRAY);
			} else {
				grayMat = img;
			}
			
			// the 11x11 dictates the smallest image space square size allowed
			// in other words, if your smallest square is 11x11 pixels, then set this to 11x11
			cornerSubPix(grayMat, pointBuf, cv::Size(11, 11),  cv::Size(-1,-1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1 ));
			
			imagePoints.push_back(pointBuf);
		} else {
			ofLog(OF_LOG_ERROR, "Calibration::add() failed, maybe your boardSize is wrong or the image has poor lighting?");
		}
		return found;
	}
	bool Calibration::clean(float minReprojectionError) {
		int removed = 0;
		for(int i = size() - 1; i >= 0; i--) {
			if(getReprojectionError(i) > minReprojectionError) {
				objectPoints.erase(objectPoints.begin() + i);
				imagePoints.erase(imagePoints.begin() + i);
				removed++;
			}
		}
		if(size() > 0) {
			if(removed > 0) {
				return calibrate();
			} else {
				return true;
			}
		} else {
			ofLog(OF_LOG_ERROR, "Calibration::clean() removed the last object/image point pair");
			return false;
		}
	}
	bool Calibration::calibrate() {
		Mat cameraMatrix = Mat::eye(3, 3, CV_64F);
    distCoeffs = Mat::zeros(8, 1, CV_64F);
    
		updateObjectPoints();
		
		int calibFlags = 0;
    float rms = calibrateCamera(objectPoints, imagePoints, addedImageSize, cameraMatrix, distCoeffs, boardRotations, boardTranslations, calibFlags);
    ofLog(OF_LOG_VERBOSE, "calibrateCamera() reports RMS error of " + ofToString(rms));
		cout << "should be printing..." << endl;
		
    bool ok = checkRange(cameraMatrix) && checkRange(distCoeffs);
		
		if(!ok) {
			ofLog(OF_LOG_ERROR, "Calibration::calibrate() failed to calibrate the camera");
		}
		
		if (ok)
			_isReady = true;
    
		distortedIntrinsics.setup(cameraMatrix, addedImageSize);
		updateReprojectionError();
		updateUndistortion();
		
		return ok;
	}
	bool Calibration::calibrateFromDirectory(string directory) {
		ofDirectory dirList;
		ofImage cur;
		dirList.listDir(directory);
		for(int i = 0; i < dirList.size(); i++) {
			cur.loadImage(dirList.getPath(i));
			if(!add(toCv(cur))) {
				ofLog(OF_LOG_ERROR, "Calibration::add() failed on " + dirList.getPath(i));
			}
		}
		return calibrate();
	}
	void Calibration::undistort(Mat img) {
		img.copyTo(undistortBuffer);
		undistort(undistortBuffer, img);
	}
	void Calibration::undistort(Mat src, Mat dst) {
		remap(src, dst, undistortMapX, undistortMapY, INTER_LINEAR);
	}
	
	ofVec2f Calibration::undistort(ofVec2f &src)
	{
		ofVec2f dst;
		
		Mat matSrc = Mat(1, 1, CV_32FC2, &src.x);
		Mat matDst = Mat(1, 1, CV_32FC2, &dst.x);;
		
		undistortPoints(matSrc, matDst, distortedIntrinsics.getCameraMatrix(), distCoeffs);
		
		return dst;
		
	}
	
	void Calibration::undistort(vector<ofVec2f> &src, vector<ofVec2f> &dst)
	{
		int nPoints = src.size();
		
		if (dst.size() != nPoints)
			dst.resize(src.size());
		
		Mat matSrc = Mat(nPoints, 1, CV_32FC2, &src[0].x);
		Mat matDst = Mat(nPoints, 1, CV_32FC2, &dst[0].x);
		
		undistortPoints(matSrc, matDst, distortedIntrinsics.getCameraMatrix(), distCoeffs);
		
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
		Mat fundamentalMatrix, essentialMatrix;
		Mat cameraMatrix = distortedIntrinsics.getCameraMatrix();
		Mat dstCameraMatrix = dst.getDistortedIntrinsics().getCameraMatrix();
		// uses CALIB_FIX_INTRINSIC by default
		stereoCalibrate(objectPoints,
										imagePoints, dst.imagePoints,
										cameraMatrix, distCoeffs,
										dstCameraMatrix, dst.distCoeffs,
										distortedIntrinsics.getImageSize(), rotation, translation,
										essentialMatrix, fundamentalMatrix);			
	}		
	float Calibration::getReprojectionError() const {
		return reprojectionError;
	}
	float Calibration::getReprojectionError(int i) const {
		return perViewErrors[i];
	}
	const Intrinsics& Calibration::getDistortedIntrinsics() const {
		return distortedIntrinsics;
	}
	const Intrinsics& Calibration::getUndistortedIntrinsics() const {
		return undistortedIntrinsics;
	}
	Mat Calibration::getDistCoeffs() const {
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
			ofCircle(toOf(imagePoints[i][j]), 5);
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
			ofTranslate(toOf(objectPoints[i][j]));
			ofCircle(0, 0, .5);
			ofPopMatrix();
		}
		glBegin(GL_LINE_STRIP);
		for(int j = 0; j < objectPoints[i].size(); j++) {
			ofVec3f cur = toOf(objectPoints[i][j]);
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
			projectPoints(Mat(objectPoints[i]), boardRotations[i], boardTranslations[i], distortedIntrinsics.getCameraMatrix(), distCoeffs, imagePoints2);
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
		Mat undistortedCameraMatrix = getOptimalNewCameraMatrix(distortedIntrinsics.getCameraMatrix(), distCoeffs, distortedIntrinsics.getImageSize(), fillFrame ? 0 : 1);
		initUndistortRectifyMap(distortedIntrinsics.getCameraMatrix(), distCoeffs, Mat(), undistortedCameraMatrix, distortedIntrinsics.getImageSize(), CV_16SC2, undistortMapX, undistortMapY);
		undistortedIntrinsics.setup(undistortedCameraMatrix, distortedIntrinsics.getImageSize());
	}
}	