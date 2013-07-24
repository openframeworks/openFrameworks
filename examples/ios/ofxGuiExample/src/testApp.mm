#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	

    //ofSetOrientation(OF_ORIENTATION_90_LEFT); 
	ofSetVerticalSync(true);
	
	// we add this listener before setting up so the initial circle resolution is correct
	circleResolution.addListener(this, &testApp::circleResolutionChanged);
	ringButton.addListener(this,&testApp::ringButtonPressed);

	// change default sizes for ofxGui so it's usable in small/high density screens
	ofxGuiSetFont("Questrial-Regular.ttf",10,true,true);
	ofxGuiSetTextPadding(4);
	ofxGuiSetDefaultWidth(300);
	ofxGuiSetDefaultHeight(18);

	gui.setup("panel"); // most of the time you don't need a name but don't forget to call setup
	gui.add(filled.set("bFill", true));
	gui.add(radius.set( "radius", 140, 10, 300 ));
	gui.add(center.set("center",ofVec2f(ofGetWidth()*.5,ofGetHeight()*.75),ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight())));
	gui.add(color.set("color",ofColor(100,100,140),ofColor(0,0),ofColor(255,255)));
	gui.add(circleResolution.set("circleRes", 5, 3, 90));
	gui.add(twoCircles.setup("twoCircles"));
	gui.add(ringButton.setup("ring"));
	gui.add(screenSize.set("screenSize", ofToString(ofGetWidth()) + "x" + ofToString(ofGetHeight())));
	
	bHide = true;

	ring.loadSound("ring.wav");

    //screenSize = ofToString(w) + "x" + ofToString(h);
}

//--------------------------------------------------------------
void testApp::circleResolutionChanged(int & circleResolution){
	ofSetCircleResolution(circleResolution);
}

//--------------------------------------------------------------
void testApp::ringButtonPressed(){
	ring.play();
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient(ofColor::white, ofColor::gray);
    
	if( filled ){
		ofFill();
	}else{
		ofNoFill();
	}

	ofSetColor(color);
	if(twoCircles){
		ofCircle(center->x-radius*.5, center->y, radius );
		ofCircle(center->x+radius*.5, center->y, radius );
	}else{
		ofCircle((ofVec2f)center, radius );
	}
	
	if( bHide ){
		gui.draw();
	}
}

//--------------------------------------------------------------
void testApp::exit(){
	ringButton.removeListener(this,&testApp::ringButtonPressed);
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

