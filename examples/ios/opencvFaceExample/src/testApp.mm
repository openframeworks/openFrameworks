#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	
	ofxiPhoneSetOrientation(OFXIPHONE_ORIENTATION_LANDSCAPE_LEFT);

	ofSetFrameRate(20);
	grabber.setDesiredFrameRate(20);
	grabber.initGrabber(480, 360);
	
	colorCv.allocate(grabber.getWidth(), grabber.getHeight());
	colorCvSmall.allocate(grabber.getWidth()/4, grabber.getHeight()/4);
	grayCv.allocate(grabber.getWidth()/4, grabber.getHeight()/4);

	finder.setup("haarcascade_frontalface_default.xml");
	finder.setNeighbors(1);
	finder.setScaleHaar(1.5);
	
}

//--------------------------------------------------------------
void testApp::update(){
	ofBackground(255,255,255);	
	
	grabber.update();
	colorCv = grabber.getPixels();
	
	colorCvSmall.scaleIntoMe(colorCv, CV_INTER_NN);
	grayCv = colorCvSmall;

	finder.findHaarObjects(grayCv);	
	faces = finder.blobs;
}

//--------------------------------------------------------------
void testApp::draw(){	
	
	ofSetColor(255);
	grabber.draw(0, 0);
	
	ofPushStyle();
		ofNoFill();
		ofSetColor(255, 0, 255);
		for(int k = 0; k < faces.size(); k++){
			ofRectangle rect(faces[k].boundingRect.x*4.0, faces[k].boundingRect.y*4.0, faces[k].boundingRect.width*4, faces[k].boundingRect.width*4.0 );
			ofRect(rect);
		}
	ofPopStyle();
	
	ofEnableAlphaBlending();
	ofSetColor(230, 0, 255, 200);
	ofRect(0, 0, ofGetWidth(), 16);
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("face detector: hold this way up", 5, 12);
}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void testApp::touchCancelled(ofTouchEventArgs& args){

}

