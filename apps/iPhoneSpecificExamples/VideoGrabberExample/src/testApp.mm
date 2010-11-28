#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	
	ofxiPhoneSetOrientation(OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT);

	ofSetFrameRate(30);

	// register touch events
	ofRegisterTouchEvents(this);
	
	grabber.initGrabber(480, 360);
	tex.allocate(grabber.getWidth(), grabber.getHeight(), GL_RGB);
	
	pix = new unsigned char[ (int)( grabber.getWidth() * grabber.getHeight() * 3.0)];
}

//--------------------------------------------------------------
void testApp::update(){
	ofBackground(255,255,255);		
	
	unsigned char * src = grabber.getPixels();
	
	int totalPix = grabber.getWidth() * grabber.getHeight() * 3;
	
	for(int k = 0; k < totalPix; k+= 3){
		pix[k  ] = 255 - src[k];
		pix[k+1] = 255 - src[k+1];
		pix[k+2] = 255 - src[k+2];		
	}
	
	tex.loadData(pix, grabber.getWidth(), grabber.getHeight(), GL_RGB);
}

//--------------------------------------------------------------
void testApp::draw(){	
	
	ofSetColor(0xFFFFFF);
	grabber.draw(0, 0);
	
	tex.draw(0, 0, tex.getWidth() / 4, tex.getHeight() / 4);
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
