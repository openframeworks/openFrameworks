#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	
	ofSetOrientation(OF_ORIENTATION_90_RIGHT);//Set iOS to Orientation Landscape Right
	
	ofBackground(255,255,255);	

	letter = '$';

	testFont.loadFont("Batang.ttf", 60, true, true, true);
	testFont2.loadFont("cooperBlack.ttf", 18, true, true, true);
	testChar = testFont.getCharacterAsPoints(letter);

}

//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(0, 90, 60);
	ofFill();

	ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate()), 10, 10);
	ofDrawBitmapString("slide your finger to see a letter as a texture \nand as a vector. ", 10, 24);

	testFont2.drawString("Hello - I am bitmap", 15, 230);

	ofFill();
	testFont2.drawStringAsShapes("Hello - I am vector", 15, 270);
	ofNoFill();
	testFont2.drawStringAsShapes("Hello - I am vector", 15, 310);

	//lets draw the key pressed as a tex and a vector both fill and no fill
	//here we show how easy it is to get

	string str = "";
	str += char(letter);

	testFont.drawString(str, 30, ofGetHeight()/2);

	//okay lets get the character back as shapes
	testChar.setFilled(true);
    testChar.draw(130, ofGetHeight()/2);

	ofNoFill();
	ofPushMatrix();
		ofTranslate(230,  ofGetHeight()/2, 0);
		ofBeginShape();
			for(int k = 0; k <(int)testChar.getOutline().size(); k++){
				if( k!= 0)ofNextContour(true) ;
				for(int i = 0; i < (int)testChar.getOutline()[k].size(); i++){
					ofVertex(testChar.getOutline()[k].getVertices()[i].x, testChar.getOutline()[k].getVertices()[i].y);
				}
			}
		ofEndShape( true );
	ofPopMatrix();

}

//--------------------------------------------------------------
void ofApp::exit(){
    
}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){
	if( touch.id == 0 ){
		letter = ofMap(touch.x, 10.0, ofGetWidth(), 33, 126, true);
		testChar = testFont.getCharacterAsPoints(letter);
	}
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
