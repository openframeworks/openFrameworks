#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	bikers.loadImage("images/bikers.jpg");
	gears.loadImage("images/gears.gif");
	tdf.loadImage("images/tdf_1972_poster.jpg");

	tdfSmall.loadImage("images/tdf_1972_poster.jpg");
	tdfSmall.resize(tdfSmall.width / 4, tdfSmall.height / 4);
	tdfSmall.setImageType(OF_IMAGE_GRAYSCALE);

	transparency.loadImage("images/transparency.png");
	bikeIcon.loadImage("images/bike_icon.png");
	bikeIcon.setImageType(OF_IMAGE_GRAYSCALE);
}

//--------------------------------------------------------------
void testApp::update(){
	ofBackground(255);	
}

//--------------------------------------------------------------
void testApp::draw(){	
	ofSetColor(255);

	bikers.draw(0, 0);
	gears.draw(600, 0);
	tdf.draw(600, 300);
	
	ofSetColor(220, 50, 50);
	tdfSmall.draw(200, 300);
	
	ofSetColor(255);
	ofEnableAlphaBlending();
	float wave = sin(ofGetElapsedTimef());
	transparency.draw(500 + (wave * 100), 20);
	ofDisableAlphaBlending();
	
	// getting the ofColors from an image,
	// using the brightness to draw circles
	int w = bikeIcon.getWidth();
	int h = bikeIcon.getHeight();
	float diameter = 10;
	ofSetColor(255, 0, 0);
	for(int y = 0; y < h; y++) {
		for(int x = 0; x < w; x++) {
			ofColor cur = bikeIcon.getColor(x, y);
			float size = 1 - (cur.getBrightness() / 255);
			ofCircle(x * diameter, 500 + y * diameter, 1 + size * diameter / 2);
		}
	}
	
	// same as above, but this time
	// use the raw data directly with getPixels()
	unsigned char* pixels = bikeIcon.getPixels();
	ofSetColor(0, 0, 255);
	for(int y = 0; y < h; y++) {
		for(int x = 0; x < w; x++) {
			int index = y * w + x;
			unsigned char cur = pixels[index];
			float size = 1 - ((float) cur / 255);
			ofCircle(200 + x * diameter, 500 + y * diameter, 1 + size * diameter / 2);
		}
	}
	
	ofSetColor(255);
	bikeIcon.draw(190, 490, 20, 20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

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
