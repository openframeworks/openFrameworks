#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	
	ofxiPhoneSetOrientation(OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT);
	
	bikers.loadImage("images/bikers.jpg");
	gears.loadImage("images/gears.gif");
	tdf.loadImage("images/tdf_1972_poster.jpg");
	tdfSmall.loadImage("images/tdf_1972_poster.jpg");
	tdfSmall.resize(tdf.width / 4, tdf.height / 4);
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
void testApp::exit(){
    
}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void testApp::touchCancelled(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void testApp::lostFocus(){
    
}

//--------------------------------------------------------------
void testApp::gotFocus(){
    
}

//--------------------------------------------------------------
void testApp::gotMemoryWarning(){
    
}

//--------------------------------------------------------------
void testApp::deviceOrientationChanged(int newOrientation){
    
}


