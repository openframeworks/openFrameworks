#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	

}

//--------------------------------------------------------------
void testApp::update(){

	if (loader.getStatus() == OF_URL_FILE_LOADER_LOADING_SUCCEEDED){
		printf("got an image -- %i bytes \n", loader.getData()->numBytes);
		
		ofLoadImageFromMemory(pix, loader.getData()->buffer, loader.getData()->numBytes);
		img.setFromPixels(pix.getPixels(), pix.getWidth(), pix.getHeight(), pix.ofImageType);
		
		loader.resetStatus();
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	
	ofSetColor(0, 0, 0);
	ofDrawBitmapString("hit spacebar to load image from web", 10, ofGetHeight()/2);
	
	float divider = ofMap( mouseX, 0, ofGetWidth(), 1, 48, true );
	
	for(int y = 0; y < pix.getHeight(); y+= divider){
		for(int x = 0; x < pix.getWidth(); x+=divider){
			ofColor c = pix.getPixel(x, y);
		
			ofSetColor( c.r, c.g, c.b );
			ofCircle( x, y, divider/2 );
		}
	}
	
	ofSetColor(255, 255, 255);
	img.draw(0, img.getHeight());	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	loader.getBytes("http://i.cdn.turner.com/cnn/2011/OPINION/01/10/carter.sudan.future/tzleft.jimmy.carter.sudan.pres.talk.afpgi.jpg");

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

