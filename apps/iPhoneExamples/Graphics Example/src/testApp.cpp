
#include "testApp.h"



//--------------------------------------------------------------
void testApp::setup(){	
	// initialize the accelerometer
	ofxAccelerometer.setup();
	
	// register touch events
	ofAddListener(ofEvents.touchDown, this, &testApp::touchDown);
	ofAddListener(ofEvents.touchUp, this, &testApp::touchUp);
	ofAddListener(ofEvents.touchMoved, this, &testApp::touchMoved);
	ofAddListener(ofEvents.touchDoubleTap, this, &testApp::touchDoubleTap);
	
	//iPhoneAlerts will be sent to this.
	ofxiPhoneAlerts.addListener(this);
	
	
	ofBackground(50, 50, 50);
	bikers.loadImage("images/bikers.jpg");
	printf("bikers h %i w %i\n", bikers.width, bikers.height );
	bikers.resize( bikers.width/3, bikers.height/3);
	gears.loadImage("images/gears.gif");
	gears.resize( gears.width/2, gears.height/2);
	tdf.loadImage("images/tdf_1972_poster.jpg");
	//	tdf.setImageType(OF_IMAGE_GRAYSCALE);
	tdfSmall.loadImage("images/tdf_1972_poster.jpg");
	tdfSmall.resize(tdf.width / 4, tdf.height / 4);
	tdfSmall.setImageType(OF_IMAGE_GRAYSCALE);
	transparency.loadImage("images/transparency.png");
	bikeIcon.loadImage("images/bike_icon.png");
	bikeIcon.setImageType(OF_IMAGE_GRAYSCALE);
}


//--------------------------------------------------------------
void testApp::update() 
{

}

//--------------------------------------------------------------
void testApp::draw()
{
	ofSetColor(0xFFFFFF);
	
	bikers.draw(10,10);
	gears.draw(200,0);
	tdf.draw(100,300);
	
	ofSetColor(0xDD3333);
	tdfSmall.draw(50,300);
	
	ofSetColor(0xFFFFFF);
	ofEnableAlphaBlending();
	transparency.draw(sin(ofGetElapsedTimeMillis()/1000.0f) * 100 + 50,20);
	ofDisableAlphaBlending();
	
	
	ofSetColor(0x000000);
	
	// getting the pixels out of an image, 
	// and then use the values to draw circles
	unsigned char * pixels = bikeIcon.getPixels();
	int w = bikeIcon.width;
	int h = bikeIcon.height;
	for (int i = 0; i < w; i++){
		for (int j = 0; j < h; j++){
			int value = pixels[j * w + i];
			float pct = 1 - (value / 255.0f);
			ofCircle(i*10,10 + j*10,1 + 5*pct);		
		}
	}
	
	ofSetColor(0xFFFFFF);
	bikeIcon.draw(100,200, 20,20);
}

//--------------------------------------------------------------
void testApp::exit() {
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


//--------------------------------------------------------------
void testApp::lostFocus() {
}

//--------------------------------------------------------------
void testApp::gotFocus() {
}

//--------------------------------------------------------------
void testApp::gotMemoryWarning() {
}


