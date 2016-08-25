#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	ofBackground(0);

	// initialize the accelerometer
	ofxAccelerometer.setup();

	// initialize the compass
	hasCompass = compass.startHeading();

}

//--------------------------------------------------------------
void testApp::update(){

	heading = compass.getTrueHeading();
	heading += 90.; // Fix related to magnometer position on device. (Samsung Galaxy SIII)
}

//--------------------------------------------------------------
void testApp::draw(){

	ofSetColor(255,255,255);

	if(hasCompass > 0) {

		ofSetColor(255);
		ofDrawBitmapString("Compass found!", 8, 20);

		//ofEnableAlphaBlending();

		ofPushMatrix();
			ofTranslate(ofGetWidth()/2, ofGetHeight()/2, 0);
			ofNoFill();
			ofCircle(0,0,100);
			ofRotateZ(heading);
			ofBeginShape();
			 	 ofVertex(0, -50);
			 	 ofVertex(-20, 60);
			 	 ofVertex(0, 50);
			 	 ofVertex(20, 60);
			 ofEndShape(true);
		ofPopMatrix();

		//ofDisableAlphaBlending();

	} else {

		ofSetColor(255);
		ofDrawBitmapString("Compass not found on device.", 8, 20);

	}

}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::touchDown(int x, int y, int id){

}

//--------------------------------------------------------------
void testApp::touchMoved(int x, int y, int id){

}

//--------------------------------------------------------------
void testApp::touchUp(int x, int y, int id){

}

//--------------------------------------------------------------
void testApp::touchDoubleTap(int x, int y, int id){

}

//--------------------------------------------------------------
void testApp::touchCancelled(int x, int y, int id){

}

//--------------------------------------------------------------
void testApp::swipe(ofxAndroidSwipeDir swipeDir, int id){

}

//--------------------------------------------------------------
void testApp::pause(){

}

//--------------------------------------------------------------
void testApp::stop(){

}

//--------------------------------------------------------------
void testApp::resume(){

}

//--------------------------------------------------------------
void testApp::reloadTextures(){

}

//--------------------------------------------------------------
bool testApp::backPressed(){
	return false;
}

//--------------------------------------------------------------
void testApp::okPressed(){

}

//--------------------------------------------------------------
void testApp::cancelPressed(){

}
