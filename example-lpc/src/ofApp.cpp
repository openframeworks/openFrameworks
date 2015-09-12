#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
    distorted.load("distorted.jpg");
    undistortedReference.load("undistorted.jpg");
	
    float imageWidth = 5616; // ImageWidth, pixels
    float imageHeight = 3744; // ImageLength, pixels
    float focalLength = 28; // FocalLength, mm
    float cropFactor = 0.975939; // SensorFormatFactor, "focal length multiplier", "crop factor"
    float focalLengthX = 0.778962; // FocalLengthX
    float focalLengthY = 0.778962; // FocalLengthY
    float principalPointX = 0.500000; // ImageXCenter, ratio
    float principalPointY = 0.500000; // ImageYCenter, ratio
    
    float k1 = -0.147131; // RadialDistortParam1
    float k2 = 0.084927; // RadialDistortParam2
    calibration.setDistortionCoefficients(k1, k2, 0, 0);
    
    Intrinsics intrinsics;
    cv::Point2d sensorSize(35 * cropFactor, 35 * cropFactor * imageHeight / imageWidth);
    cv::Size imageSize(distorted.getWidth(), distorted.getHeight());
    intrinsics.setup(focalLength, imageSize, sensorSize);
    calibration.setFillFrame(false);
    calibration.setIntrinsics(intrinsics);
    
	imitate(undistorted, distorted);
    
    Mat distortedMat = toCv(distorted);
    Mat undistortedMat = toCv(undistorted);
    calibration.undistort(distortedMat, undistortedMat);
    undistorted.update();
}

void ofApp::update() {
}

void ofApp::draw() {
    float scale = ofGetHeight() / distorted.getHeight();
    ofScale(scale, scale);
    distorted.draw(0, 0);
    if(ofGetKeyPressed('r')) {
        undistortedReference.draw(0, 0);
    }
    if(ofGetKeyPressed('u')) {
        undistorted.draw(0, 0);
    }
}
