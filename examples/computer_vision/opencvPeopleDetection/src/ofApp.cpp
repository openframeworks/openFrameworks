#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	//note: this detector needs a fairly clean background to work well
	bLiveVideo = false;
	
	if( bLiveVideo ){
		vidGrabber.setVerbose(true);
		vidGrabber.setup(640, 480);
		
		colorImg.allocate(vidGrabber.getWidth(),vidGrabber.getHeight());
	}else{
		vidPlayer.load("people.mov");
		vidPlayer.play();
		vidPlayer.setLoopState(OF_LOOP_NORMAL);

		colorImg.allocate(vidPlayer.getWidth(),vidPlayer.getHeight());
	}
	
	//just loads the detector
	detector.setup();
	
	//we resize the image down to 320 width to get better performance
	detector.setResizeWidth(320);
	
	//this should be the minimum dimensions a person could be in the input image
	//needs to be a multiple of 8
	detector.setMinPersonSize(64, 128);
		
}

//--------------------------------------------------------------
void ofApp::update(){
	ofBackground(30, 30, 30);

	bool bNewFrame = false;

	if( bLiveVideo ){
		vidGrabber.update();
		bNewFrame = vidGrabber.isFrameNew();
		if( bNewFrame ){
			colorImg.setFromPixels(vidGrabber.getPixels());
		}
	}else{
		vidPlayer.update();
		bNewFrame = vidPlayer.isFrameNew();
		if( bNewFrame ){
			colorImg.setFromPixels(vidPlayer.getPixels());
		}
	}

	if (bNewFrame){
		//can be a ofCvColorImage or a ofxCvGrayscaleImage
		cv::Mat frame = cv::cvarrToMat(colorImg.getCvImage());
		
		//get back the people detected as a vector of ofRectangle
		peopleRects = detector.detect(frame);
	}

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofPushMatrix();
		ofTranslate(20, 20, 0);

		ofSetColor(255, 255, 255);
		colorImg.draw(0, 0);
		
		ofNoFill();
		ofSetColor(200, 20, 240);
		for( auto & rect : peopleRects ){
			ofDrawRectangle(rect);
		}
	
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

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
