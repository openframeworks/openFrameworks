#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	 
	
	int screenW = ofGetScreenWidth();
	int screenH = ofGetScreenHeight();
	ofSetWindowPosition(screenW/2-300/2, screenH/2-300/2);
	
	// load our typeface
	vagRounded.loadFont("vag.ttf", 16);

	bFullscreen	= 0;
	
	// lets set the initial window pos
	// and background color
	// ofSetVerticalSync(true);
	ofSetFrameRate(60);
	
	ofBackground(50,50,50);	
	
	
	ballPositionX = 150;
	ballPositionY = 150;
	ballVelocityX = ofRandom(-5,5);
	ballVelocityY = ofRandom(-5,5);
}

//--------------------------------------------------------------
void ofApp::update(){

	// update our window title with the framerate and the position of the window
	// [zach fix] ofSetWindowTitle(ofToString(ofGetFrameRate(), 2)+":fps - pos ("+ofToString((int)windowX)+","+ofToString((int)windowY)+")");

	
	if(bFullscreen){
		ofHideCursor();
	}else{
		ofShowCursor();
	}
	
	
	ballPositionX += ballVelocityX;
	ballPositionY += ballVelocityY;
	
	int posx = ofGetWindowPositionX();
	int posy = ofGetWindowPositionY();
	
	if (ballPositionX < 0){
		ballPositionX = 0;
		ballVelocityX *= -1;
		if (!bFullscreen){
			ofSetWindowPosition(posx-10, posy);
		}
	} else if (ballPositionX > ofGetWidth()){
		ballPositionX = ofGetWidth();
		ballVelocityX *= -1;
		if (!bFullscreen){
			ofSetWindowPosition(posx+10, posy);
		}
	}
	
	if (ballPositionY < 0){
		ballPositionY = 0;
		ballVelocityY *= -1;
		if (!bFullscreen){
			ofSetWindowPosition(posx, posy-10);
		}
	} else if (ballPositionY > ofGetHeight()){
		ballPositionY = ofGetHeight();
		ballVelocityY *= -1;
		if (!bFullscreen){
			ofSetWindowPosition(posx, posy+10);
		}
	}
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetupScreen();
	
	ofSetHexColor(0x999999);
	
	// lets show our window pos in pixels
	// macs actually start the Y pos from 40
	vagRounded.drawString("window pos ("+ofToString(ofGetWindowPositionX())+", "+ofToString( ofGetWindowPositionY())+")", 10, 25);

	if(!bFullscreen){
		vagRounded.drawString("press f to enter fullscreen", -140 + ofGetWidth()/2, ofGetHeight()/2);	
		vagRounded.drawString("window is normal", -100 + ofGetWidth()/2, ofGetHeight() - 10);
	} else {
		vagRounded.drawString("press f to exit fullscreen", -150 + ofGetWidth()/2, ofGetHeight()/2); 
		vagRounded.drawString("window is fullscreen", -140 + ofGetWidth()/2, ofGetHeight() - 10); 
	}
	

	ofSetHexColor(0xFFFFFF);
	ofCircle(ballPositionX, ballPositionY, 15);
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){ 

	if(key == 'f'){
	
		bFullscreen = !bFullscreen;
		
		if(!bFullscreen){
			ofSetWindowShape(300,300);
			ofSetFullscreen(false);
			// figure out how to put the window in the center:
			int screenW = ofGetScreenWidth();
			int screenH = ofGetScreenHeight();
			ofSetWindowPosition(screenW/2-300/2, screenH/2-300/2);
		} else if(bFullscreen == 1){
			ofSetFullscreen(true);
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
