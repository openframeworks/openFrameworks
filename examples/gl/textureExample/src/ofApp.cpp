#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	


	w = 250;
	h = 200;
	
	
	
	grayPixels.allocate(w,h,OF_PIXELS_GRAY);
	colorPixels.allocate(w,h,OF_PIXELS_RGB);
	colorAlphaPixels.allocate(w,h,OF_PIXELS_RGBA);


	// gray pixels, set them randomly
	for (int i = 0; i < w*h; i++){
		grayPixels[i] = ofRandomuf() * 255;
		
	}
	
	// color pixels, use x and y to control red and green
	for (int y = 0; y < h; y++){
		for (int x = 0; x < w; x++){
			colorPixels.setColor(x,y,ofColor(x,y,0));
		}
	}
	
	// color alpha pixels, use x to control alpha
	for (int y = 0; y < h; y++){
		for (int x = 0; x < w; x++){
			colorAlphaPixels.setColor(x,y,ofColor(255,133,200,x));
		}
	}

	texGray.allocate(grayPixels);
	texColor.allocate(colorPixels);
	texColorAlpha.allocate(colorAlphaPixels);

}
 

//--------------------------------------------------------------
void ofApp::update(){
	
	ofBackground(255,255,255);
	
	for (int i = 0; i < w; i++){
		for (int j = 0; j < h; j++){
			grayPixels[j*w+i] = (unsigned char)(ofRandomuf() * 255);
		}
	}
	texGray.loadData(grayPixels.getData(), w,h, GL_LUMINANCE);
	
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofSetHexColor(0xffffff);
	
	texGray.draw(100,100,w,h);
	texColor.draw(350,300,w,h);
	
	// 	blending had to be enabled 
	// 	for transparency to work:
	
	ofEnableAlphaBlending();
	texColorAlpha.draw(250,200,w,h);
	ofDisableAlphaBlending();

}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){ 
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	
	// when the mouse moves, we change the color image:
	float pct = (float)x / (float)ofGetWidth();
	for (int y = 0; y < h; y++){
		for (int x = 0; x < w; x++){
			colorPixels.setColor(x,y,ofColor(x,y,pct*255));
		}
	}
	// finally, load those pixels into the texture
	texColor.loadData(colorPixels);
	
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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
