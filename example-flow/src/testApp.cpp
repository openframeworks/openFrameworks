#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	camera.initGrabber(320, 240);
	prev.allocate(camera.getWidth(), camera.getHeight(), OF_IMAGE_GRAYSCALE);
	next.allocate(camera.getWidth(), camera.getHeight(), OF_IMAGE_GRAYSCALE);
	
	panel.setup(250, 800);
	
	panel.addPanel("Optical Flow");
	
	//panel.addPanel("calcOpticalFlowFarneback");
	panel.addSlider("pyrScale", .5, 0, 1);
	panel.addSlider("levels", 4, 1, 8, true);
	panel.addSlider("winsize", 8, 4, 64, true);
	panel.addSlider("iterations", 2, 1, 8, true);
	panel.addSlider("polyN", 7, 5, 10, true);
	panel.addSlider("polySigma", 1.5, 1.1, 2);
	panel.addToggle("OPTFLOW_FARNEBACK_GAUSSIAN", false);
	
	//panel.addPanel("calcOpticalFlowPyrLK");
	panel.addToggle("useFarneback", true);
	panel.addSlider("winSize", 32, 4, 64, true);
	panel.addSlider("maxLevel", 3, 0, 8, true);
	
	panel.addSlider("maxFeatures", 200, 1, 1000);
	panel.addSlider("qualityLevel", 0.01, 0.001, .02);
	panel.addSlider("minDistance", 4, 1, 16);
}

void testApp::update(){
	camera.update();
	
	if(camera.isFrameNew()) {
		prev.setFromPixels(next.getPixels(), next.getWidth(), next.getHeight(), OF_IMAGE_GRAYSCALE);
		next.setFromPixels(camera.getPixels(), camera.getWidth(), camera.getHeight(), OF_IMAGE_COLOR);
		next.setImageType(OF_IMAGE_GRAYSCALE);

		if(panel.getValueB("useFarneback")) {
			calcOpticalFlowFarneback(
				toCv(prev),
				toCv(next),
				flow,
				
				panel.getValueF("pyrScale"),
				panel.getValueF("levels"),
				panel.getValueF("winsize"),
				panel.getValueF("iterations"),
				panel.getValueF("polyN"),
				panel.getValueF("polySigma"),
				panel.getValueB("OPTFLOW_FARNEBACK_GAUSSIAN") ? OPTFLOW_FARNEBACK_GAUSSIAN : 0
			);
		} else {
			prevPts.clear();
			// to avoid detecting in empty spaces, use the mask
			goodFeaturesToTrack(
				toCv(prev),
				prevPts,
				panel.getValueI("maxFeatures"),
				panel.getValueF("qualityLevel"),
				panel.getValueF("minDistance"));
			
			vector<uchar> status;
			vector<float> err;
			int winSize = panel.getValueI("winSize");
			calcOpticalFlowPyrLK(
				toCv(prev),
				toCv(next),
				prevPts,
				nextPts,
				status,
				err,
				
				cv::Size(winSize, winSize),
				panel.getValueI("maxLevel")
			);
		}
	}
}

ofVec2f getFlowOffset(const Mat& flow, int x, int y) {
	const Vec2f& vec = flow.at<Vec2f>(y, x);
	return ofVec2f(vec[1], vec[0]);
}

ofVec2f getFlowPosition(const Mat& flow, int x, int y) {
	const Vec2f& vec = flow.at<Vec2f>(y, x);
	return ofVec2f(x + vec[1], y + vec[0]);
}

void drawFlow(const Mat& flow, unsigned int stepSize) {
	for(int i = 0; i < flow.rows; i += stepSize) {
		for(int j = 0; j < flow.cols; j += stepSize) {
			ofVec2f cur(j, i);
			ofLine(cur, getFlowPosition(flow, j, i));
		} 
	}
}

void drawFeatures(vector<Point2f>& prevPts, vector<Point2f>& nextPts) {
	for(int i = 0; i < prevPts.size(); i++) {
		ofLine(toOf(prevPts[i]), toOf(nextPts[i]));
	}
}

void testApp::draw(){
	ofBackground(0);
	
	ofPushMatrix();
	
	ofSetColor(255);
	ofTranslate(400, 100);
	prev.draw(0, 0);
	next.draw(0, 240);
	
	if(panel.getValueB("useFarneback")) {
		drawFlow(flow, 4);
	} else {
		drawFeatures(prevPts, nextPts);
	}
	
	ofPopMatrix();
}