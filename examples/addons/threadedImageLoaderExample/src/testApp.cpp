#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetVerticalSync(true);
	total = 24;
	images.resize(total*2);
	for(int i = 0; i < total; ++i) {
		loader.loadFromDisk(images[i*2], "of" + ofToString(i) + ".png");
		loader.loadFromURL(images[i*2+1], "http://www.openframeworks.cc/of_inverted.png");
	}
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){	
	
	// draw the images.
	ofSetColor(255);
	for(int i = 0; i < (int)images.size(); ++i) {
		int x = (i%8);
		int y = (i/8);
		images[i].draw(x*128,y*128, 128,128);
	}	
	
	// draw the FPS
	ofRect(0,ofGetHeight()-20,30,20);

	ofSetColor(0);
	ofDrawBitmapString(ofToString(ofGetFrameRate(),0),5,ofGetHeight()-5);
}

//--------------------------------------------------------------
void testApp::exit(){
	loader.stopThread();
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
