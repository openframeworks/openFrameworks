#include "testApp.h"
#include "ofPixelUtils.h";

//--------------------------------------------------------------
void testApp::setup(){
	
	
	temp.loadImage("frog-1.jpg");
	
	
	//temp.setImageType(OF_IMAGE_COLOR_ALPHA);
	//tex.allocate(150,150, temp.getOFPixels().getGlDataType());

	//ofPixels tempPix;
	//ofTexture tex;
	
	

}

//--------------------------------------------------------------
void testApp::update(){

	
	//tempPix = temp.getOFPixels();
	//tex.loadData(tempPix);
}

//--------------------------------------------------------------
void testApp::draw(){

	temp.draw(0,0);
	temp2.draw(temp.width,0);
	//tex.draw(400,50);
//	
//	rotateTex.draw(600,0);
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

	mousePressedPt.set(x,y);
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

	
	float dx = x - mousePressedPt.x;
	float dy = y - mousePressedPt.y;
	
	temp2.cropFrom(temp, mousePressedPt.x, mousePressedPt.y, dx, dy);
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

