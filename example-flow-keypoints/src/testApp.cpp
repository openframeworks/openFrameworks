#include "testApp.h"

using namespace cv;
using namespace ofxCv;

void testApp::setup(){
	grabber.initGrabber(640,480);
}

void testApp::update(){
	grabber.update();
	if(grabber.isFrameNew()){
		flow.calcOpticalFlow(grabber);
	}
}

void testApp::draw(){
	grabber.draw(0,0);
	flow.draw();
	if(ofGetMousePressed()){
		ofNoFill();
		ofRect(rect);
	}
}

void testApp::mouseDragged(int x, int y, int button){
	ofVec2f p2(x,y);
	rect.set(p1,p2.x-p1.x,p2.y-p1.y);
}

void testApp::mousePressed(int x, int y, int button){
	p1.set(x,y);
}

void testApp::mouseReleased(int x, int y, int button){
	ofVec2f p2(x,y);
	rect.set(p1,p2.x-p1.x,p2.y-p1.y);
	vector<KeyPoint> keypoints;
	vector<KeyPoint> keypointsInside;
	vector<cv::Point2f> featuresToTrack;
	FAST(toCv(grabber),keypoints,2);
	for(int i=0;i<keypoints.size();i++){
		if(rect.inside(toOf(keypoints[i].pt))){
			keypointsInside.push_back(keypoints[i]);
		}
	}
    #if CV_MAJOR_VERSION>=2 && (CV_MINOR_VERSION>4 || (CV_MINOR_VERSION==4 && CV_SUBMINOR_VERSION>=1))
	KeyPointsFilter::retainBest(keypointsInside,30);
    #endif
	KeyPoint::convert(keypointsInside,featuresToTrack);
	flow.setFeaturesToTrack(featuresToTrack);
}

