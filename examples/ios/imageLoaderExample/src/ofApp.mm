#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	
	ofSetOrientation(OF_ORIENTATION_90_RIGHT);//Set iOS to Orientation Landscape Right
	
	bikers.load("images/bikers.jpg");
	gears.load("images/gears.gif");
	tdf.load("images/tdf_1972_poster.jpg");
	tdfSmall.load("images/tdf_1972_poster.jpg");
	tdfSmall.resize(tdf.width / 4, tdf.height / 4);
	tdfSmall.setImageType(OF_IMAGE_GRAYSCALE);
	transparency.load("images/transparency.png");
	bikeIcon.load("images/bike_icon.png");
	bikeIcon.setImageType(OF_IMAGE_GRAYSCALE);
}

//--------------------------------------------------------------
void ofApp::update(){
	ofBackground(255,255,255);	
}

//--------------------------------------------------------------
void ofApp::draw(){	

	ofScale(0.5, 0.5, 1.0);

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
	unsigned char * pixels = bikeIcon.getPixels();
	int w = bikeIcon.width;
	int h = bikeIcon.height;
	for (int i = 0; i < w; i++){
		for (int j = 0; j < h; j++){
			int value = pixels[j * w + i];
			float pct = 1 - (value / 255.0f);
			ofCircle(i*10,500 + j*10,1 + 5*pct);		
		}
	}
	
	ofSetHexColor(0xFFFFFF);
	bikeIcon.draw(300,500, 20,20);
}

//--------------------------------------------------------------
void ofApp::exit(){
    
}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){

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


