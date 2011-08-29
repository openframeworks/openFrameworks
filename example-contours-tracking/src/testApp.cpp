#include "testApp.h"

float trackingDistance(const cv::Rect& a, const cv::Rect& b) {
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	float dw = a.width - b.width;
	float dh = a.height - b.height;
	float pd = sqrtf(dx * dx + dy * dy);
	float sd = sqrtf(dw * dw + dh * dh);
	return pd + sd;
}

float trackingDistance(const cv::Point2f& a, const cv::Point2f& b) {
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	return sqrtf(dx * dx + dy * dy);
}

void testApp::setup() {
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
		
		vector<cv::Rect> boundingBoxes;
		for(int i = 0; i < contourFinder.size(); i++) {
			boundingBoxes.push_back(contourFinder.getBoundingRect(i));
		} 
		rectTracker.track(boundingBoxes);
	}
}

void testApp::draw() {
	ofSetColor(255);
	movie.draw(0, 0);
	contourFinder.draw();
}
