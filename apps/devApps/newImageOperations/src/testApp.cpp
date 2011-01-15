#include "testApp.h"
#include "ofPixelUtils.h";

//--------------------------------------------------------------
void testApp::setup(){
	
	
	temp.loadImage("frog-1.jpg");
	temp.setImageType(OF_IMAGE_COLOR_ALPHA);
	tex.allocate(150,150, temp.getOFPixels().getGlDataType());

	ofPixels tempPix;
	ofTexture tex;
	
	

}

//--------------------------------------------------------------
void testApp::update(){

	
	tempPix = temp.getOFPixels();
	ofPixelUtils::crop(tempPix, mouseX, mouseY, 150,150);
	tex.loadData(tempPix);
}

//--------------------------------------------------------------
void testApp::draw(){

	temp.draw(0,0);
	
	tex.draw(400,50);
	
	rotateTex.draw(600,0);
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

	rotatePix =  temp.getOFPixels();
	ofPixelUtils::rotate90(rotatePix, x / 100);
	rotateTex.allocate(rotatePix.getWidth(), rotatePix.getHeight(), rotatePix.getGlDataType());
	rotateTex.loadData(rotatePix);
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

