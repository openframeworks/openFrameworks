#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	
	// register touch events
	ofRegisterTouchEvents(this);

	ofxiPhoneSetOrientation(OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT);
	
	ofBackground(255,255,255);	

	letter = '$';

	testFont.loadFont("Batang.ttf", 80, true, true, true);
	testFont2.loadFont("cooperBlack.ttf", 26, true, true, true);

}

//--------------------------------------------------------------
void testApp::update(){
	
}

//--------------------------------------------------------------
void testApp::draw(){

	ofSetColor(0, 90, 60);
	ofFill();

	ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate()), 10, 10);
	ofDrawBitmapString("slide finger to change letter", 10, 26);

	testFont2.drawString("Hello - I am bitmap", 15, 190);
	ofFill();
	testFont2.drawStringAsShapes("Hello - I am vector", 15, 240);
	ofNoFill();
	testFont2.drawStringAsShapes("Hello - I am vector", 15, 285);

	//lets draw the key pressed as a tex and a vector both fill and no fill
	//here we show how easy it is to get

	string str = "";
	str += char(letter);

	testFont.drawString(str, 10, 140);

	//okay lets get the character back as ofPoints
	//all curves are calculated for us so all we need to do is draw!
	ofTTFCharacter testChar;
	testChar = testFont.getCharacterAsPoints(letter);

	ofFill();
	ofPushMatrix();
		ofTranslate(100, 140, 0);
		ofBeginShape();
			for(int k = 0; k <testChar.contours.size(); k++){
				if( k!= 0)ofNextContour(true);
				for(int i = 0; i < testChar.contours[k].pts.size(); i++){
					ofVertex(testChar.contours[k].pts[i].x, testChar.contours[k].pts[i].y);
				}
			}
		ofEndShape( true );
	ofPopMatrix();

	ofNoFill();
	ofPushMatrix();
		ofTranslate(200, 140, 0);
		ofBeginShape();
			for(int k = 0; k <testChar.contours.size(); k++){
				if( k!= 0)ofNextContour(true);
				for(int i = 0; i < testChar.contours[k].pts.size(); i++){
					ofVertex(testChar.contours[k].pts[i].x, testChar.contours[k].pts[i].y);
				}
			}
		ofEndShape( true );
	ofPopMatrix();

}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs &touch){
	if( touch.id == 0 ){
		letter = ofMap(touch.x, 10.0, ofGetWidth(), 33, 126, true);
	}
}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void testApp::touchCancelled(ofTouchEventArgs& args){

}

