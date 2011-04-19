#include "testApp.h"

const float diffThreshold = 2;

void testApp::setup() {
	ofSetVerticalSync(true);
	cam.initGrabber(640, 480);
	
	imitate(undistorted, cam);
	imitate(previous, cam);
	imitate(diff, cam);
	
	lastRecording = false;
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
		bool recording = diffMean < diffThreshold;
		
		if(recording && !lastRecording) {
			if(calibration.add(camMat)) {
				cout << "re-calibrating" << endl;
				calibration.calibrate();
				calibration.save("calibration.yml");
			}
		}
		
		lastRecording = recording;
	
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
	
	ofSetColor(0);	
	stringstream distCoeffs;
	distCoeffs << calibration.getDistCoeffs();
	ofDrawBitmapString(ofToString(diffMean), 10, 20);
	ofDrawBitmapString(distCoeffs.str(), 10, 40);
	ofDrawBitmapString(ofToString(calibration.getReprojectionError()), 10, 60);
}
