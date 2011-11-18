#include "ofxCv/Calibration.h"
#include "ofxCv/Helpers.h"
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
	
	void Intrinsics::loadProjectionMatrix(float nearDist, float farDist) const {
		glViewport(0, 0, imageSize.width, imageSize.height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		float w = imageSize.width;
		float h = imageSize.height;
		float fx = cameraMatrix.at<double>(0, 0);
		float fy = cameraMatrix.at<double>(1, 1);
		float cx = principalPoint.x;
		float cy = principalPoint.y;
		glFrustum(
			nearDist * (-cx) / fx, nearDist * (w - cx) / fx,
			nearDist * (cy - h) / fy, nearDist * (cy) / fy,
			nearDist, farDist);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(
			0, 0, 0,
			0, 0, 1,
			0, -1, 0);
	}
	
	Calibration::Calibration() :
		patternSize(cv::Size(10, 7)), squareSize(2.5), // based on Chessboard_A4.pdf, assuming world units are centimeters
		subpixelSize(cv::Size(11,11)),
		fillFrame(true),
		_isReady(false),
		reprojectionError(0.0)
	{
		
	}
	
	void Calibration::save(string filename, bool absolute) const {
		if(!_isReady){
			ofLog(OF_LOG_ERROR, "Calibration::save() failed, because your calibration isn't ready yet!");
		}
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
		fs << "features" << "[";
		for(int i = 0; i < imagePoints.size(); i++) {
			fs << "{:" << "points" << "[:"; 
			for( int j = 0; j < imagePoints[i].size(); j++ ){
				fs << imagePoints[i][j].x << imagePoints[i][j].y;
			}
			fs << "]" << "}";
		}
		fs << "]";
	}
	
	void Calibration::load(string filename, bool absolute) {
		imagePoints.clear();
		FileStorage fs(ofToDataPath(filename, absolute), FileStorage::READ);
		cv::Size imageSize, sensorSize;
		Mat cameraMatrix;
		int pointCount;
		FileNode features;
		fs["cameraMatrix"] >> cameraMatrix;
		fs["imageSize_width"] >> imageSize.width;
		fs["imageSize_height"] >> imageSize.height;
		fs["sensorSize_width"] >> sensorSize.width;
		fs["sensorSize_height"] >> sensorSize.height;
		fs["distCoeffs"] >> distCoeffs;
		fs["reprojectionError"] >> reprojectionError;
		vector<float> points;		
		features = fs["features"];
		int idx = 0;

		for(FileNodeIterator it = features.begin(); it != features.end(); it++) {
			idx++;
			(*it)["points"] >> points;
			vector<Point2f> featureset;
			for(int i = 0; i < points.size(); i+=2){
				featureset.push_back(Point2f(points[i], points[i+1]));
			}
			imagePoints.push_back(featureset); // technique 1
		}
		addedImageSize = imageSize;
	}
	void Calibration::setPatternSize(int xCount, int yCount) {
		patternSize = cv::Size(xCount, yCount);
	}
	void Calibration::setSquareSize(float squareSize) {
		this->squareSize = squareSize;
	}
	void Calibration::setFillFrame(bool fillFrame) {
		this->fillFrame = fillFrame;
	}
	void Calibration::setSubpixelSize(int subpixelSize) {
		subpixelSize = MAX(subpixelSize,2);
		this->subpixelSize = cv::Size(subpixelSize,subpixelSize);
	}
	bool Calibration::add(Mat img) {
		addedImageSize = img.size();
		
		vector<Point2f> pointBuf;
		
		// find corners
		bool found = findBoard(img, pointBuf, true);
		
		if (found)
			imagePoints.push_back(pointBuf);
		else
			ofLog(OF_LOG_ERROR, "Calibration::add() failed, maybe your patternSize is wrong or the image has poor lighting?");
		return found;
	}
	bool Calibration::findBoard(Mat img, vector<Point2f> &pointBuf, bool refine) {
		// no CV_CALIB_CB_FAST_CHECK, because it breaks on dark images (e.g., dark IR images from kinect)
		int chessFlags = CV_CALIB_CB_ADAPTIVE_THRESH;// | CV_CALIB_CB_NORMALIZE_IMAGE;
		bool found = findChessboardCorners(img, patternSize, pointBuf, chessFlags);
		
		// improve corner accuracy
		if(found) {
			if(img.type() != CV_8UC1) {
				cvtColor(img, grayMat, CV_RGB2GRAY);
			} else {
				grayMat = img;
			}
			
			// the subpixelSize dictates the smallest image space square size allowed
			// in other words, if your smallest square is 11x11 pixels, then set this to 11x11
			cornerSubPix(grayMat, pointBuf, subpixelSize,  cv::Size(-1,-1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1 ));
			
			return true;
		} else {
			return false;
		}
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

		_isReady = checkRange(cameraMatrix) && checkRange(distCoeffs);
	
		if(!_isReady) {
			ofLog(OF_LOG_ERROR, "Calibration::calibrate() failed to calibrate the camera");
		}
		
		distortedIntrinsics.setup(cameraMatrix, addedImageSize);
		updateReprojectionError();
		updateUndistortion();
		
		return _isReady;
	}
	
	bool Calibration::isReady(){
		return _isReady;
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
	
	bool Calibration::getTransformation(Calibration& dst, Mat& rotation, Mat& translation) {
		//if(imagePoints.size() == 0 || dst.imagePoints.size() == 0) {
		if(!_isReady) {
			ofLog(OF_LOG_ERROR, "getTransformation() requires both Calibration objects to have just been calibrated");
			return false;
		}
		if(imagePoints.size() != dst.imagePoints.size() || patternSize != dst.patternSize) {
			ofLog(OF_LOG_ERROR, "getTransformation() requires both Calibration objects to be trained simultaneously on the same board");
			return false;
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
		return true;
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
	cv::Size Calibration::getPatternSize() const {
		return patternSize;
	}
	float Calibration::getSquareSize() const {
		return squareSize;
	}
	void Calibration::customDraw() {
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

		ofMesh mesh;
		mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
		for(int j = 0; j < objectPoints[i].size(); j++) {
			ofVec3f cur = toOf(objectPoints[i][j]);
			mesh.addVertex(cur);
		}
		mesh.draw();
		
		ofPopMatrix();
		ofPopStyle();
	}
	void Calibration::updateObjectPoints() {
		vector<Point3f> points = createObjectPoints(patternSize, squareSize, CHESSBOARD);
		objectPoints.resize(imagePoints.size(), points);
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
	
	vector<Point3f> Calibration::createObjectPoints(cv::Size patternSize, float squareSize, CalibrationPattern patternType) {
		vector<Point3f> corners;
		switch(patternType) {
			case CHESSBOARD:
			case CIRCLES_GRID:
				for(int i = 0; i < patternSize.height; i++)
					for(int j = 0; j < patternSize.width; j++)
						corners.push_back(Point3f(float(j * squareSize), float(i * squareSize), 0));
				break;
			case ASYMMETRIC_CIRCLES_GRID:
				for(int i = 0; i < patternSize.height; i++)
					for(int j = 0; j < patternSize.width; j++)
						corners.push_back(Point3f(float(((2 * j) + (i % 2)) * squareSize), float(i * squareSize), 0));
				break;
		}
		return corners;
	}
}	
