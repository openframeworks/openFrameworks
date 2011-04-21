#include "testApp.h"

const float diffThreshold = 2; // maximum amount of movement
const float timeThreshold = 1; // minimum time between snapshots
const int startCleaning = 10; // start cleaning outliers after this many samples

void testApp::setup() {
	ofSetVerticalSync(true);
	cam.initGrabber(640, 480);
	
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

void drawHighlightString(string text, int x, int y, ofColor background = ofColor::black, ofColor foreground = ofColor::white) {
	int textWidth =  10 + text.length() * 8;
	ofSetColor(background);
	ofFill();
	ofRect(x - 5, y - 12, textWidth, 20);
	ofSetColor(foreground);
	ofDrawBitmapString(text, x, y);
}

void testApp::draw() {
	ofSetColor(255);
	cam.draw(0, 0);
	undistorted.draw(640, 0);
	
	stringstream distCoeffs;
	distCoeffs << calibration.getDistCoeffs();
	drawHighlightString("dist coeffs: " + distCoeffs.str(), 10, 20, ofColor::fromHex(0xffee00), ofColor(0));
	drawHighlightString("movement: " + ofToString(diffMean), 10, 40, ofColor::fromHex(0x00abec));
	drawHighlightString("reproj error: " + ofToString(calibration.getReprojectionError()) + " from " + ofToString(calibration.size()), 10, 60, ofColor::fromHex(0xec008c));
	for(int i = 0; i < calibration.size(); i++) {
		drawHighlightString(ofToString(i) + ": " + ofToString(calibration.getReprojectionError(i)), 10, 80 + 16 * i, ofColor::fromHex(0xec008c));
	}
}

void testApp::keyPressed(int key) {
	if(key == ' ') {
		active = !active;
	}
}
