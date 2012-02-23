#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetVerticalSync(true);
	total = 24;
	
	for(int i = 0; i < total; ++i) {			
		ofImage* img = new ofImage();
		images.push_back(img);
		loader.loadFromDisk(img, "of" + ofToString(i) + ".png");

		ofImage* url_img = new ofImage();
		images.push_back(url_img);
		loader.loadFromURL(images.back(), "http://www.roxlu.com/assets/images/of_inverted.png");
	}
	
	//TODO: no! this should be internal - overloading << doesn't make sense
	//cout << loader << endl;
	
	loader.startThread(false, false);
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){	
	
	// draw the images.
	glColor3f(1,1,1);
	for(int i = 0; i < images.size(); ++i) {
		int x = (i%8);
		int y = (i/8);
		images.at(i)->draw(x*128,y*128, 128,128);
	}	
	
	// draw the FPS
	glColor3f(1,1,1);
	ofRect(0,ofGetHeight()-20,30,20);
	glColor3f(0,0,0);
	ofDrawBitmapString(ofToString(ofGetFrameRate(),0),5,ofGetHeight()-5);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

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

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
