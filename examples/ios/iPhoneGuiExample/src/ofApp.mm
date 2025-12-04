#include "ofApp.h"
#include "MyGuiView.h"

MyGuiView * myGuiViewController;

//--------------------------------------------------------------
void ofApp::setup(){	
	//NOTE WE WON'T RECEIVE TOUCH EVENTS INSIDE OUR APP WHEN THERE IS A VIEW ON TOP OF THE OF VIEW
    //However, if you resize the GUI view in MyGuiView, you will receive touch events in areas outside the GUI view.

	lengthRatio	= 0.5;
	numPoints	= 5;
	bFill		= true;
	
	//Our Gui setup
    UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"MyGuiView" bundle:nil];
    myGuiViewController	=  [storyboard instantiateViewControllerWithIdentifier:@"MyGuiView"];
	[ofxiOSGetGLParentView() addSubview:myGuiViewController.view];

	ofBackground(255,255,255);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	
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
		float angle = -glm::half_pi<float>() + ofMap(k, 0, numPoints*2, 0, glm::two_pi<float>(), true);
		float tx = std::cos(angle) * rad[k % 2];
		float ty = std::sin(angle) * rad[k % 2];
		ofVertex(x + tx, y + ty);
	}
	ofEndShape(true);
	
}

//--------------------------------------------------------------
void ofApp::exit(){
	
}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){
    //You can check the touch event.
    cout<<"touchDown:"<<touch.id<<endl;
}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch){
	
}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs & touch){
    //IF THE VIEW IS HIDDEN LETS BRING IT BACK!
    if( myGuiViewController.view.hidden ){
        myGuiViewController.view.hidden = NO;
    }
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
