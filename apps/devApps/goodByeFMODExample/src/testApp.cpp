#include "testApp.h"
#include "ofSoundPlayer_.h"

ofSoundPlayer_  blah;

//--------------------------------------------------------------
void testApp::setup(){

	
	
	
	
	blah.loadSound("beat.wav");
	
	//blah.enableSpectrumCalculation(10);
	//blah.setSpeed(0);

}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){

	
	ofFill();
	ofSetColor(0);
	
	/*
	FFTResults results = blah.getSpectrum();
	int divAmount = results.numberOfBands;
	for (int i = 0; i < divAmount; i++){
		ofRect(ofGetWidth()/divAmount * i, 0, ofGetWidth()/divAmount, ofGetHeight()/2.0 *(1 - results.fftValuesL[i]));
		ofRect(ofGetWidth()/divAmount * i, ofGetHeight()/2.0, ofGetWidth()/divAmount, ofGetHeight()/2.0 *(1 - results.fftValuesR[i]));
	}
	*/
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	//blah.setSpeed(0);
	//blah.tryMultiPlay();
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
	blah.play();
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

