#include "testApp.h"

using namespace ofxCv;
using namespace cv;

const float dyingTime = 1;

void Glow::setup(const cv::Rect& track) {
	color.setHsb(ofRandom(0, 255), 255, 255);
	cur = toOf(track).getCenter();
	smooth = cur;
}

void Glow::update(const cv::Rect& track) {
	cur = toOf(track).getCenter();
	smooth.interpolate(cur, .5);
	all.addVertex(smooth);
}

void Glow::kill() {
	float curTime = ofGetElapsedTimef();
	if(startedDying == 0) {
		startedDying = curTime;
	} else if(curTime - startedDying > dyingTime) {
		dead = true;
	}
}

void Glow::draw() {
	ofPushStyle();
	float size = 16;
	ofSetColor(255);
	if(startedDying) {
		ofSetColor(ofColor::red);
		size = ofMap(ofGetElapsedTimef() - startedDying, 0, dyingTime, size, 0, true);
	}
	ofNoFill();
	ofCircle(cur, size);
	ofSetColor(color);
	all.draw();
	ofSetColor(255);
	ofDrawBitmapString(ofToString(label), cur);
	ofPopStyle();
}

void testApp::setup() {
	ofSetVerticalSync(true);
	ofBackground(0);
	
	movie.loadMovie("video.mov");
	movie.play();
	
	contourFinder.setMinAreaRadius(1);
	contourFinder.setMaxAreaRadius(100);
	contourFinder.setThreshold(15);
	
	// wait for half a frame before forgetting something
	tracker.setPersistence(15);
	// an object can move up to 50 pixels per frame
	tracker.setMaximumDistance(50);
}

void testApp::update() {
	movie.update();
	if(movie.isFrameNew()) {
		blur(movie, 10);
		contourFinder.findContours(movie);
		tracker.track(contourFinder.getBoundingRects());
	}
}

void testApp::draw() {	
	ofSetColor(255);
	movie.draw(0, 0);
	contourFinder.draw();
	vector<Glow>& followers = tracker.getFollowers();
	for(int i = 0; i < followers.size(); i++) {
		followers[i].draw();
	}
}
