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
	ofBackground(255,255,255);	
}

//--------------------------------------------------------------
void testApp::draw(){	
	ofSetHexColor(0xFFFFFF);

	bikers.draw(0,0);
	gears.draw(600,0);
	tdf.draw(600,300);
	
	ofSetHexColor(0xDD3333);
	tdfSmall.draw(200,300);
	
	ofSetHexColor(0xFFFFFF);
	ofEnableAlphaBlending();
	transparency.draw(sin(ofGetElapsedTimeMillis()/1000.0f) * 100 + 500,20);
	ofDisableAlphaBlending();
	
	
	ofSetHexColor(0x000000);
	
	// getting the pixels out of an image, 
	// and then use the values to draw circles
	int w = bikeIcon.width;
	int h = bikeIcon.height;
	float diameter = 10;
	for(int y = 0; y < h; y++) {
		for(int x = 0; x < w; x++) {
			ofColor cur = bikeIcon.getPixel(x, y);
			float size = 1 - (cur.getBrightness() / 255);
			ofCircle(x * diameter, 500 + y * diameter, 1 + size * diameter / 2);
		}
	}
	
	ofSetHexColor(0xFFFFFF);
	bikeIcon.draw(300,500, 20,20);
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
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
