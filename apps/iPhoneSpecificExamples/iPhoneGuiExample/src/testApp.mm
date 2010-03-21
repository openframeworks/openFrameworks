#include "testApp.h"

#include "MyGuiView.h"
MyGuiView * myGuiViewController;

//these are static variables which we use to communicate with the gui class.
//kind of ghetto but easier than messages/events for simple stuff

float lengthRatio	= 0.5;
int numPoints		= 5;
bool bFill			= true;

//--------------------------------------------------------------
void testApp::setup(){	
	// register touch events
	ofRegisterTouchEvents(this);
		
	//NOTE WE WON'T RECEIVE TOUCH EVENTS INSIDE OUR APP WHEN THERE IS A VIEW ON TOP OF THE OF VIEW

	//Our Gui setup
	myGuiViewController	= [[MyGuiView alloc] initWithNibName:@"MyGuiView" bundle:nil];
	[[[UIApplication sharedApplication] keyWindow] addSubview:myGuiViewController.view];

	ofBackground(255,255,255);
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	
	float width		= ofGetWidth();
	float height	= ofGetHeight();
	float x			= width/2;
	float y			= height/2;
	
	float size		= width * 0.7;

	//we have two radii for our star shape!
	float rad[2];
	rad[0]			= size * 0.5;
	rad[1]			= rad[0] * lengthRatio;
	
	ofSetColor(255, 50, 10);
	
	if( bFill )ofFill();
	else ofNoFill();
	
	ofBeginShape();
	for(int k = 0; k < numPoints*2; k++){
		float angle = -HALF_PI + ofMap(k, 0, numPoints*2, 0, TWO_PI, true);
		float tx = cos(angle) * rad[k % 2];
		float ty = sin(angle) * rad[k % 2];
		ofVertex(x + tx, y + ty);
	}
	ofEndShape(true);
	
}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs &touch){

	//IF THE VIEW IS HIDDEN LETS BRING IT BACK!
	if( myGuiViewController.view.hidden ){
		myGuiViewController.view.hidden = NO;
	}
	
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
