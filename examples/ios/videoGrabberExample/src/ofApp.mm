#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	
	ofSetOrientation(OF_ORIENTATION_90_RIGHT);//Set iOS to Orientation Landscape Right

	ofSetFrameRate(30);

	grabber.initGrabber(480, 360, OF_PIXELS_BGRA);
	tex.allocate(grabber.getWidth(), grabber.getHeight(), GL_RGB);
	
	pix = new unsigned char[ (int)( grabber.getWidth() * grabber.getHeight() * 3.0) ];
}

//--------------------------------------------------------------
void ofApp::update(){
	ofBackground(255,255,255);	
	
	grabber.update();
	
	unsigned char * src = grabber.getPixels();
	int totalPix = grabber.getWidth() * grabber.getHeight() * 3;
	
	for(int k = 0; k < totalPix; k+= 3){
		pix[k  ] = 255 - src[k];
		pix[k+1] = 255 - src[k+1];
		pix[k+2] = 255 - src[k+2];		
	}
	
	tex.loadData(pix, grabber.getWidth(), grabber.getHeight(), GL_RGB);
}

//--------------------------------------------------------------
void ofApp::draw(){	
	
	ofSetColor(255);
	grabber.draw(0, 0);
	
	tex.draw(0, 0, tex.getWidth() / 4, tex.getHeight() / 4);
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


