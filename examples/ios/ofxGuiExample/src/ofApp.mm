#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	

    //ofSetOrientation(OF_ORIENTATION_90_LEFT); 
	ofSetVerticalSync(true);
	
	// we add this listener before setting up so the initial circle resolution is correct
	circleResolution.addListener(this, &ofApp::circleResolutionChanged);
	ringButton.addListener(this,&ofApp::ringButtonPressed);

	// change default sizes for ofxGui so it's usable in small/high density screens
	ofxGuiSetFont("Questrial-Regular.ttf",10,true,true);
	ofxGuiSetTextPadding(4);
	ofxGuiSetDefaultWidth(300);
	ofxGuiSetDefaultHeight(18);

	gui.setup("panel"); // most of the time you don't need a name but don't forget to call setup
	gui.add(filled.set("bFill", true));
	gui.add(radius.set( "radius", 140, 10, 300 ));
	gui.add(center.set("center",glm::vec2(ofGetWidth()*.5,ofGetHeight()*.75),glm::vec2(0,0),glm::vec2(ofGetWidth(),ofGetHeight())));
	gui.add(color.set("color",ofColor(100,100,140),ofColor(0,0),ofColor(255,255)));
	gui.add(circleResolution.set("circleRes", 5, 3, 90));
	gui.add(twoCircles.setup("twoCircles"));
	gui.add(ringButton.setup("ring"));
	gui.add(screenSize.set("screenSize", ofToString(ofGetWidth()) + "x" + ofToString(ofGetHeight())));
	
	bHide = true;

	ring.load("ring.wav");

    //screenSize = ofToString(w) + "x" + ofToString(h);
}

//--------------------------------------------------------------
void ofApp::circleResolutionChanged(int & circleResolution){
	ofSetCircleResolution(circleResolution);
}

//--------------------------------------------------------------
void ofApp::ringButtonPressed(){
	ring.play();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackgroundGradient(ofColor::white, ofColor::gray);
    
	if( filled ){
		ofFill();
	}else{
		ofNoFill();
	}

	ofSetColor(color);
	if(twoCircles){
		ofDrawCircle(center->x-radius*.5, center->y, radius );
		ofDrawCircle(center->x+radius*.5, center->y, radius );
	}else{
		ofDrawCircle((glm::vec2)center, radius );
	}
	
	if( bHide ){
		gui.draw();
	}
}

//--------------------------------------------------------------
void ofApp::exit(){
	ringButton.removeListener(this,&ofApp::ringButtonPressed);
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

