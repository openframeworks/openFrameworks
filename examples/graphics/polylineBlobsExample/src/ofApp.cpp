#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	threshold = 32;

	camWidth = 640;
	camHeight = 480;

	cam.setup(camWidth, camHeight);
	cvImgColor.allocate(camWidth, camHeight);
	cvImgGrayscale.allocate(camWidth, camHeight);

	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
}

//--------------------------------------------------------------
void ofApp::update() {
	ofSetLogLevel(OF_LOG_VERBOSE);

	cam.update();
	if(cam.isFrameNew()) {
		cam.update();
		cvImgColor.setFromPixels(cam.getPixels().getData(), camWidth, camHeight);
		cvImgGrayscale.setFromColorImage(cvImgColor);
		cvImgGrayscale.threshold(threshold, ofGetKeyPressed());
		contourFinder.findContours(cvImgGrayscale, 64 * 64, camWidth * camHeight, 5, false, true);

		polylines.clear();
		smoothed.clear();
		resampled.clear();
		boundingBoxes.clear();
		closestPoints.clear();
		closestIndices.clear();

		glm::vec3 target (mouseX, mouseY,0);

		for(unsigned int i = 0; i < contourFinder.blobs.size(); i++) {
			ofPolyline cur;
			// add all the current vertices to cur polyline
			cur.addVertices(contourFinder.blobs[i].pts);
			cur.setClosed(true);

			// add the cur polyline to all these vector<ofPolyline>
			polylines.push_back(cur);
			smoothed.push_back(cur.getSmoothed(8));
			resampled.push_back(cur.getResampledByCount(100).getSmoothed(8));

			boundingBoxes.push_back(cur.getBoundingBox());

			unsigned int index;
			closestPoints.push_back(resampled.back().getClosestPoint(target, &index));
			closestIndices.push_back(index);
		}
	}
}

//--------------------------------------------------------------
void drawWithNormals(const ofPolyline& polyline) {
	for(int i=0; i< (int) polyline.size(); i++ ) {
		bool repeatNext = i == (int)polyline.size() - 1;

		const glm::vec3& cur = polyline[i];
		const glm::vec3& next = repeatNext ? polyline[0] : polyline[i + 1];

		float angle = ofRadToDeg(atan2f(next.y - cur.y, next.x - cur.x));
		float distance = glm::distance(cur, next);

		if(repeatNext) {
			ofSetColor(255, 0, 255);
		}
		ofPushMatrix();
		ofTranslate(cur.x, cur.y, 0);
		ofRotateDeg(angle);
		ofDrawLine(0, 0, 0, distance);
		ofDrawLine(0, 0, distance, 0);
		ofPopMatrix();
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground(0);

	ofSetColor(255);
	cvImgGrayscale.draw(0, 0);

	for(unsigned int i = 0; i < polylines.size(); i++) {
		ofNoFill();

		ofSetColor(255, 0, 0);
		drawWithNormals(polylines[i]);

		ofSetColor(0, 255, 0);
		drawWithNormals(smoothed[i]);

		ofSetColor(0, 0, 255);
		drawWithNormals(resampled[i]);

		ofSetColor(0, 255, 255);
		ofDrawRectangle(boundingBoxes[i]);

		ofSetColor(255, 0, 0);
		ofDrawLine(closestPoints[i], glm::vec3(mouseX, mouseY,0));
		ofSetColor(0, 0, 255);
		ofDrawLine(resampled[i][closestIndices[i]], glm::vec3(mouseX, mouseY,0));
	}

	ofSetColor(255, 0, 0);
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()) + " fps", 10, 20);
	ofDrawBitmapString("Click and drag to set a new threshold.", 10, 40);
	ofDrawBitmapString("Hold down any key to invert the thresholding.", 10, 60);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	threshold = x;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
