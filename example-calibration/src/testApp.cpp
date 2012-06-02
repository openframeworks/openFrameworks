#include "testApp.h"

using namespace ofxCv;
using namespace cv;

const float diffThreshold = 2.5; // maximum amount of movement
const float timeThreshold = 1; // minimum time between snapshots
const int startCleaning = 10; // start cleaning outliers after this many samples

void testApp::setup() {
	ofSetVerticalSync(true);
	cam.initGrabber(640, 480);
	
	FileStorage settings(ofToDataPath("settings.yml"), FileStorage::READ);
	if(settings.isOpened()) {
		int xCount = settings["xCount"], yCount = settings["yCount"];
		calibration.setPatternSize(xCount, yCount);
		float squareSize = settings["squareSize"];
		calibration.setSquareSize(squareSize);
		CalibrationPattern patternType;
		switch(settings["patternType"]) {
			case 0: patternType = CHESSBOARD; break;
			case 1: patternType = CIRCLES_GRID; break;
			case 2: patternType = ASYMMETRIC_CIRCLES_GRID; break;
		}
		calibration.setPatternType(patternType);
	}
	
	imitate(undistorted, cam);
	imitate(previous, cam);
	imitate(diff, cam);
	
	lastTime = 0;
	
	active = true;
}

void testApp::update() {
	cam.update();
	if(cam.isFrameNew()) {		
		Mat camMat = toCv(cam);
		Mat prevMat = toCv(previous);
		Mat diffMat = toCv(diff);
		
		absdiff(prevMat, camMat, diffMat);	
		camMat.copyTo(prevMat);
		
		diffMean = mean(Mat(mean(diffMat)))[0];
		
		float curTime = ofGetElapsedTimef();
		if(active && curTime - lastTime > timeThreshold && diffMean < diffThreshold) {
			if(calibration.add(camMat)) {
				cout << "re-calibrating" << endl;
				calibration.calibrate();
				if(calibration.size() > startCleaning) {
					calibration.clean();
				}
				calibration.save("calibration.yml");
				lastTime = curTime;
			}
		}
		
		if(calibration.size() > 0) {
			calibration.undistort(toCv(cam), toCv(undistorted));
			undistorted.update();
		}
	}
}

void testApp::draw() {
	ofSetColor(255);
	cam.draw(0, 0);
	undistorted.draw(640, 0);
	
	stringstream intrinsics;
	intrinsics << "fov: " << toOf(calibration.getDistortedIntrinsics().getFov()) << " distCoeffs: " << calibration.getDistCoeffs();
	drawHighlightString(intrinsics.str(), 10, 20, yellowPrint, ofColor(0));
	drawHighlightString("movement: " + ofToString(diffMean), 10, 40, cyanPrint);
	drawHighlightString("reproj error: " + ofToString(calibration.getReprojectionError()) + " from " + ofToString(calibration.size()), 10, 60, magentaPrint);
	for(int i = 0; i < calibration.size(); i++) {
		drawHighlightString(ofToString(i) + ": " + ofToString(calibration.getReprojectionError(i)), 10, 80 + 16 * i, magentaPrint);
	}
}

void testApp::keyPressed(int key) {
	if(key == ' ') {
		active = !active;
	}
}
