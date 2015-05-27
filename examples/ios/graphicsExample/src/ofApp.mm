#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	
	ofSetOrientation(OF_ORIENTATION_90_RIGHT);//Set iOS to Orientation Landscape Right

	counter = 0;
	ofSetCircleResolution(50);
	ofBackground(255,255,255);
	bSmooth = false;
}

//--------------------------------------------------------------
void ofApp::update(){
	counter = counter + 0.033f;
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofTranslate(0, -140, 0);
	ofScale(0.7, 0.7, 1.0);
	
	//--------------------------- circles
	//let's draw a circle:
	ofSetColor(255,130,0);
	float radius = 50 + 10 * sin(counter);
	ofFill();		// draw "filled shapes"
	ofCircle(100,400,radius);

	// now just an outline
	ofNoFill();
	ofSetHexColor(0xCCCCCC);
	ofCircle(100,400,80);

	// use the bitMap type
	// note, this can be slow on some graphics cards
	// because it is using glDrawPixels which varies in
	// speed from system to system.  try using ofTrueTypeFont
	// if this bitMap type slows you down.
	ofSetHexColor(0x000000);
	ofDrawBitmapString("circle", 75,500);

	//--------------------------- rectangles
	ofFill();
	for (int i = 0; i < 200; i++){
		ofSetColor((int)ofRandom(0,255),(int)ofRandom(0,255),(int)ofRandom(0,255));
		ofDrawRectangle(ofRandom(250,350),ofRandom(350,450),ofRandom(10,20),ofRandom(10,20));
	}
	ofSetHexColor(0x000000);
	ofDrawBitmapString("rectangles", 275,500);

	//---------------------------  transparency
	ofSetHexColor(0x00FF33);
	ofDrawRectangle(400,350,100,100);
	// alpha is usually turned off - for speed puposes.  let's turn it on!
	ofEnableAlphaBlending();
	ofSetColor(255,0,0,127);   // red, 50% transparent
	ofDrawRectangle(450,430,100,33);
	ofSetColor(255,0,0,(int)(counter * 10.0f) % 255);   // red, variable transparent
	ofDrawRectangle(450,370,100,33);
	ofDisableAlphaBlending();

	ofSetHexColor(0x000000);
	ofDrawBitmapString("transparency", 410,500);

	//---------------------------  lines
	// a bunch of red lines, make them smooth if the flag is set

	if (bSmooth){
		ofEnableSmoothing();
	}

	ofSetHexColor(0xFF0000);
	for (int i = 0; i < 20; i++){
		ofDrawLine(600,300 + (i*5),800, 250 + (i*10));
	}

	if (bSmooth){
		ofDisableSmoothing();
	}

	ofSetHexColor(0x000000);
	ofDrawBitmapString("lines\npress 's' to toggle smoothness", 600,500);

}

//--------------------------------------------------------------
void ofApp::exit(){
    
}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){
	bSmooth = true;
}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch){
	bSmooth = false;
}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchCancelled(ofTouchEventArgs & touch){
	bSmooth = false;
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
