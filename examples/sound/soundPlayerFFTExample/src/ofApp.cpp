#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){	 

	// load in sounds:
	beat.load("sounds/jdee_beat.mp3");
	ow.load("sounds/ow.mp3");
	dog.load("sounds/dog.mp3");
	rooster.load("sounds/rooster.mp3");
}


//--------------------------------------------------------------
void ofApp::update(){
	
	ofBackground(80,80,20);

	// update the sound playing system:
	ofSoundUpdate();	
	
	// (1) we increase px and py by adding vx and vy
	px += vx;
	py += vy;
	
	// (2) check for collision, and trigger sounds:
	// horizontal collisions:
	if (px < 0){
		px = 0;
		vx *= -1;
		dog.play();
	} else if (px > ofGetWidth()){
		px = ofGetWidth();
		vx *= -1;
		ow.play();
	}
	// vertical collisions:
	if (py < 0 ){
		py = 0;
		vy *= -1;
		rooster.play();
	} else if (py > ofGetHeight()){
		py = ofGetHeight();
		vy *= -1;
		beat.play();
	}
	// (3) slow down velocity:
	vx 	*= 0.996f;
	vy 	*= 0.996f;

	// (4) we use velocity for volume of the samples:
	float vel = sqrt(vx*vx + vy*vy);
	ow.setVolume(MIN(vel/5.0f, 1));
	beat.setVolume(MIN(vel/5.0f, 1));
	dog.setVolume(MIN(vel/5.0f, 1));
	rooster.setVolume(MIN(vel/5.0f, 1));

	// (5) grab the fft, and put in into a "smoothed" array,
	//		by taking maximums, as peaks and then smoothing downward
	float * val = ofSoundGetSpectrum(nBandsToGet);		// request 128 values for fft
	for (int i = 0;i < nBandsToGet; i++){
		
		// let the smoothed value sink to zero:
		fftSmoothed[i] *= 0.96f;
		
		// take the max, either the smoothed or the incoming:
		if (fftSmoothed[i] < val[i]) fftSmoothed[i] = val[i];
		
	}


}

//--------------------------------------------------------------
void ofApp::draw(){

	
	ofEnableAlphaBlending();
		ofSetColor(255,255,255,100);
		ofDrawRectangle(100,ofGetHeight()-300,5*128,200);
	ofDisableAlphaBlending();
	
	// draw the fft resutls:
	ofSetColor(255,255,255,255);
	
	float width = (float)(5*128) / nBandsToGet;
	for (int i = 0;i < nBandsToGet; i++){
		// (we use negative height here, because we want to flip them
		// because the top corner is 0,0)
		ofDrawRectangle(100+i*width,ofGetHeight()-100,width,-(fftSmoothed[i] * 200));
	}
	
	// finally draw the playing circle:

	ofEnableAlphaBlending();
		ofSetColor(255,255,255,20);
		ofDrawCircle(px, py,50);
	ofDisableAlphaBlending();
	
	ofSetHexColor(0xffffff);
	ofDrawCircle(px, py,8);
}


//--------------------------------------------------------------
void ofApp::keyPressed  (int key){ 
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	// add into vx and vy a small amount of the change in mouse:
	vx += (x - prevx) / 20.0f;
	vy += (y - prevy) / 20.0f;
	// store the previous mouse position:
	prevx = x;
	prevy = y;
}
 
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	prevx = x;
	prevy = y;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

