#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	ofSetVerticalSync(true);
	cam.initGrabber(640, 480);
	
	calibration.load("mbp-2011-isight.yml");
	patternSize = calibration.getPatternSize();
	objectPoints = Calibration::createObjectPoints(patternSize, 1., CHESSBOARD);
	found = false;
	
	light.enable();
	light.setPosition(500, 0, 0);
}

void testApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		found = calibration.findBoard(toCv(cam), imagePoints);
		if(found) {
			Mat cameraMatrix = calibration.getDistortedIntrinsics().getCameraMatrix();
			Mat rvec, tvec;
			solvePnP(Mat(objectPoints), Mat(imagePoints), cameraMatrix, calibration.getDistCoeffs(), rvec, tvec);
			modelMatrix = makeMatrix(rvec, tvec);
		}
	}
}

void testApp::draw() {
	ofSetColor(255);
	cam.draw(0, 0);
	if(found) {
		calibration.getDistortedIntrinsics().loadProjectionMatrix();
		applyMatrix(modelMatrix);
		
		ofMesh mesh;
		mesh.setMode(OF_PRIMITIVE_POINTS);
		for(int i = 0; i < objectPoints.size(); i++) {
			mesh.addVertex(toOf(objectPoints[i]));
		}
		glPointSize(3);
		ofSetColor(magentaPrint);
		mesh.drawVertices();
		
		ofEnableLighting();
		ofSetColor(255);
		glEnable(GL_DEPTH_TEST);
		ofTranslate(.5, .5, -.5);
		for(int i = 0; i < patternSize.width / 2; i++) {
			for(int j = 0; j < patternSize.height / 2; j++) {
				for(int k = 0; k < 3; k++) {
					ofBox(2 * i, 2 * j, -2 * k, 1);
				}
			}
		}
		glDisable(GL_DEPTH_TEST);
		ofDisableLighting();
	}
}
