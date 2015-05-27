#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	
	ofSetOrientation(OF_ORIENTATION_90_RIGHT);//Set iOS to Orientation Landscape Right
	
	w = 250;
	h = 200;
	
	texGray.allocate(w,h,GL_LUMINANCE);
	texColor.allocate(w,h,GL_RGB);
	texColorAlpha.allocate(w,h,GL_RGBA);
	
	grayPixels			= new unsigned char [w*h];
	colorPixels 		= new unsigned char [w*h*3];
	colorAlphaPixels	= new unsigned char [w*h*4];


	// gray pixels, set them randomly
	for (int i = 0; i < w*h; i++){
		grayPixels[i] = (unsigned char)(ofRandomuf() * 255);
		
	}
	
	// color pixels, use w and h to control red and green
	for (int i = 0; i < w; i++){
		for (int j = 0; j < h; j++){
			colorPixels[(j*w+i)*3 + 0] = i;	// r
			colorPixels[(j*w+i)*3 + 1] = j;	// g
			colorPixels[(j*w+i)*3 + 2] = 0; // b
		}
	}
	
	// color alpha pixels, use w and h to control red and green
	for (int i = 0; i < w; i++){
		for (int j = 0; j < h; j++){
			colorAlphaPixels[(j*w+i)*4 + 0] = 255;	// r
			colorAlphaPixels[(j*w+i)*4 + 1] = 133;	// g
			colorAlphaPixels[(j*w+i)*4 + 2] = 200; 	// b
			colorAlphaPixels[(j*w+i)*4 + 3] = i; 	// alpha
		}
	}
	
	
	texGray.loadData(grayPixels, w,h, GL_LUMINANCE); 
	texColor.loadData(colorPixels, w,h, GL_RGB);
	texColorAlpha.loadData(colorAlphaPixels, w,h, GL_RGBA);

}


//--------------------------------------------------------------
void ofApp::update(){
	
	ofBackground(255,255,255);
	
	for (int i = 0; i < w; i++){
		for (int j = 0; j < h; j++){
			grayPixels[j*w+i] = (unsigned char)(ofRandomuf() * 255);
		}
	}
	texGray.loadData(grayPixels, w,h, GL_LUMINANCE); 
	
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofScale(0.6, 0.6, 1.0);

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
void ofApp::exit(){
    
}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){

	// when the mouse moves, we change the color image:
	float pct = (float)touch.x / (float)ofGetWidth();
	for (int i = 0; i < w; i++){
		for (int j = 0; j < h; j++){
			colorPixels[(j*w+i)*3 + 0] = i;	// r
			colorPixels[(j*w+i)*3 + 1] = j;	// g
			colorPixels[(j*w+i)*3 + 2] = (unsigned char)(pct*255); // b
		}
	}

	// finally, load those pixels into the texture
	texColor.loadData(colorPixels, w,h, GL_RGB);

}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchCancelled(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::lostFocus(){
    
}

//--------------------------------------------------------------
void ofApp::gotFocus(){
    
}

//--------------------------------------------------------------
void ofApp::gotMemoryWarning(){
    
}

//--------------------------------------------------------------
void ofApp::deviceOrientationChanged(int newOrientation){
    
}

