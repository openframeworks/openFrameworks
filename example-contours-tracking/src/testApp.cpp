#include "testApp.h"

void testApp::setup() {
	ofSetBackgroundAuto(false);
	ofSetVerticalSync(true);
	ofBackground(0);
	
	movie.loadMovie("video.mov");
	movie.play();
	
	contourFinder.setMinAreaRadius(1);
	contourFinder.setMaxAreaRadius(100);
	contourFinder.setThreshold(15);
}

void testApp::update() {
	movie.update();
	if(movie.isFrameNew()) {
		blur(movie, 10);
		contourFinder.findContours(movie);
		
		// this should be happening inside the contour finder
		vector<cv::Rect> boundingBoxes;
		for(int i = 0; i < contourFinder.size(); i++) {
			boundingBoxes.push_back(contourFinder.getBoundingRect(i));
		}
		rectTracker.track(boundingBoxes);
	}
}

void testApp::draw() {
	ofSetColor(255);
	//movie.draw(0, 0);
	//contourFinder.draw();
	vector<unsigned int>& labels = rectTracker.getLabels();
	for(int i = 0; i < contourFinder.size(); i++) {
		ofPoint center = toOf(contourFinder.getCenter(i));
		//ofDrawBitmapString(ofToString(labels[i]), center);
	}
	for(int i = 0; i < labels.size(); i++) {
		unsigned int label = labels[i];
		if(rectTracker.existsPrevious(label)) {
			ofSeedRandom(10000 * label);
			ofSetColor(ofColor::fromHsb(ofRandom(255), 255, 255));
			cv::Rect& previous = rectTracker.getPrevious(label);
			cv::Rect& current = rectTracker.getCurrent(label);
			ofLine(
				previous.x + previous.width / 2, previous.y + previous.height / 2,
				current.x + current.width / 2, current.y + current.height / 2);
		}
	}
}
