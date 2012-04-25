#include "testApp.h"

//NOTE THIS IS A BETA movie player for iphone.
//full features of ofVideoPlayer will be implemented in the next minor release 

//--------------------------------------------------------------
void testApp::setup(){	
	ofSetFrameRate(30);
	ofBackground(225, 225, 225);

	ofBackground(255,255,255);
    
	fingerMovie.loadMovie("hands.m4v");
	fingerMovie.play();
}

//--------------------------------------------------------------
void testApp::update(){
    fingerMovie.idleMovie();
}

//--------------------------------------------------------------
void testApp::draw(){
	
	ofSetColor(54);
	ofDrawBitmapString("movieplayer example\ntouch screen to restart movie", 13, 20);

	ofDrawBitmapString("pixel access", 13, 260);


	ofTranslate(0, 35, 0);
	ofScale(0.75, 0.75);
	
	ofSetColor(255);
    fingerMovie.draw(20,20);
		
    unsigned char * pixels = fingerMovie.getPixels();
    // let's move through the "RGB" char array
    // using the red pixel to control the size of a circle.
	
	//we need to do this as the movie takes several seconds to load
	//the loading happens in the background so we check for pixels not being NULL to make sure we have real pixels to play with
	if( pixels!= NULL && fingerMovie.getWidth() > 0 ){
	
		ofSetColor(54);
		for (int i = 4; i < fingerMovie.getWidth(); i+=8){
			for (int j = 4; j < fingerMovie.getHeight(); j+=8){
				unsigned char r = pixels[(j * 320 + i)*3];
				float val = 1 - ((float)r / 255.0f);
				ofCircle(20 + i, 290 + 20+j,4*val);
			}
		}
    }
    
    ofSetColor(0);        
    if(fingerMovie.getIsMovieDone()){
        ofSetHexColor(0xFF0000);
        ofDrawBitmapString("end of movie",20,440);
    }
}

//--------------------------------------------------------------
void testApp::exit(){
    
}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs & touch){
	if( touch.id == 0 ){
		fingerMovie.loadMovie("fingers.m4v");
		fingerMovie.play();
	}
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

