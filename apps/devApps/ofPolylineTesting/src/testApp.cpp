#include "testApp.h"

void testApp::setup() {
	threshold = 32;
	
	camWidth = 640;
	camHeight = 480;
	
	cam.initGrabber(640, 480);
	cvImgColor.allocate(camWidth, camHeight);
	cvImgGrayscale.allocate(camWidth, camHeight);
	
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
}

void testApp::update() {
	ofSetLogLevel(OF_LOG_VERBOSE);

	cam.grabFrame();
	if(cam.isFrameNew()) {
		cam.update();
		cvImgColor.setFromPixels(cam.getPixels(), camWidth, camHeight);
		cvImgGrayscale.setFromColorImage(cvImgColor);
		cvImgGrayscale.threshold(threshold, ofGetKeyPressed());
		contourFinder.findContours(cvImgGrayscale, 64 * 64, camWidth * camHeight, 5, false, true);
		
		polylines.clear(); 
		smoothed.clear();
		resampled.clear();
		boundingBoxes.clear();
		closestPoints.clear();
		closestIndices.clear();
		
		ofPoint target;
		target.set(mouseX, mouseY);
			
		for(int i = 0; i < contourFinder.blobs.size(); i++) {
			ofPolyline cur;
			// add all the current vertices to cur polyline
			cur.addVertexes(contourFinder.blobs[i].pts);
			cur.setClosed(true);
			
			// add the cur polyline to all these vector<ofPolyline>
			polylines.push_back(cur);
			smoothed.push_back(ofGetSmoothed(cur, 8));
			resampled.push_back(ofGetSmoothed(ofGetResampledCount(cur, 100), 8));
			
			boundingBoxes.push_back(ofGetBoundingBox(cur));
			
			unsigned int index;
			closestPoints.push_back(ofGetClosestPoint(resampled.back(), target, &index));
			closestIndices.push_back(index);
		}
	}
}

void drawWithNormals(const ofPolyline& polyline) {
	for(int i=0; i< (int) polyline.size(); i++ ) {
		bool repeatNext = i == polyline.size() - 1;
	
		const ofPoint& cur = polyline[i];
		const ofPoint& next = repeatNext ? polyline[0] : polyline[i + 1];
		
		float angle = atan2f(next.y - cur.y, next.x - cur.x) * RAD_TO_DEG;
		float distance = cur.distance(next);
	
		if(repeatNext) {
			ofSetColor(255, 0, 255);
		}
		glPushMatrix();
		glTranslatef(cur.x, cur.y, 0);
		ofRotate(angle);
		ofLine(0, 0, 0, distance);
		ofLine(0, 0, distance, 0);
		glPopMatrix();
	}
}

void testApp::draw() {
	ofBackground(0);
	
	
	ofSetColor(255);
	cvImgGrayscale.draw(0, 0);
	
	for(int i = 0; i < polylines.size(); i++) {
		ofNoFill();
	
		ofSetColor(255, 0, 0);
		drawWithNormals(polylines[i]);
		
		ofSetColor(0, 255, 0);
		drawWithNormals(smoothed[i]);
		
		ofSetColor(0, 0, 255);
		drawWithNormals(resampled[i]);
		
		ofSetColor(0, 255, 255);
		ofRect(boundingBoxes[i]);
		
		ofSetColor(255, 0, 0);
		ofLine(closestPoints[i], ofPoint(mouseX, mouseY));
		ofSetColor(0, 0, 255);
		ofLine(resampled[i][closestIndices[i]], ofPoint(mouseX, mouseY));
	}
	
	ofSetColor(255, 0, 0);	
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()) + " fps", 10, 20);
	ofDrawBitmapString("Click and drag to set a new threshold.", 10, 40);
}

void testApp::keyPressed(int key) {
}

void testApp::keyReleased(int key) {
}

void testApp::mouseMoved(int x, int y) {
}

void testApp::mouseDragged(int x, int y, int button) {
	threshold = x;
}

void testApp::mousePressed(int x, int y, int button) {
}

void testApp::mouseReleased(int x, int y, int button) {
}

void testApp::windowResized(int w, int h) {
}

