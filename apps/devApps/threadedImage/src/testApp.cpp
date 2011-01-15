#include "testApp.h"
#include <sstream>
//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetVerticalSync(true);
	total = 24;
	for(int i = 0; i < total; ++i) {
		stringstream ss;
		ss << "of" << i << ".png";
		ofLog(OF_LOG_VERBOSE, ss.str());
		ofImage* img = new ofImage();
		loader.loadFromDisk(img,ss.str());
		images.push_back(img);
		
		ofImage* url_img = new ofImage();
		loader.loadFromURL(url_img, "http://www.roxlu.com/assets/images/of_inverted.png");
		images.push_back(url_img);
	}
	ofImage* url_img = new ofImage();
	loader.loadFromURL(url_img, "http://images.wildmadagascar.org/pictures/bemaraha/tsingy_forest.JPG");
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){	
	glColor3f(1,1,1);
	vector<ofImage*>::iterator it = images.begin();
	int i = 0;
	while(it != images.end()) {
		int x = (i%8);
		int y = (i/8);
		(*it)->draw(x*128,y*128, 128,128);
		++i;
		++it;
	}	
	glColor3f(1,1,1);
	ofRect(0,ofGetHeight()-20,100,20);
	glColor3f(0,0,0);
	ofDrawBitmapString(ofToString(ofGetFrameRate()),10,ofGetHeight()-5);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key == ' ') {
		loader.startThread(false, false);
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

