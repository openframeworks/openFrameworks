#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	synth.loadSound("sounds/synth.wav");
	beats.loadSound("sounds/1085.mp3");
	vocals.loadSound("sounds/Violet.mp3");
	synth.setVolume(0.75f);
	beats.setVolume(0.75f);
	vocals.setVolume(0.5f);
	font.loadFont("Sudbury_Basin_3D.ttf", 32);
	beats.setMultiPlay(false);
	vocals.setMultiPlay(true);
}

//--------------------------------------------------------------
void testApp::update(){

	ofBackground(255,255,255);

	// update the sound playing system:
	ofSoundUpdate();

}

//--------------------------------------------------------------
void testApp::draw(){

	// draw the background colors:
	float widthDiv = ofGetWidth() / 3.0f;
	ofSetHexColor(0xeeeeee);
	ofRect(0,0,widthDiv,ofGetHeight());
	ofSetHexColor(0xffffff);
	ofRect(widthDiv,0,widthDiv,ofGetHeight());
	ofSetHexColor(0xdddddd);
	ofRect(widthDiv*2,0,widthDiv,ofGetHeight());


	//---------------------------------- synth:
	if (synth.getIsPlaying()) ofSetHexColor(0xFF0000);
	else ofSetHexColor(0x000000);
	font.drawString("synth !!", 50,50);

	ofSetHexColor(0x000000);
	string tempStr = "click to play\npct done: "+ofToString(synth.getPosition())+"\nspeed: " + ofToString(synth.getSpeed()) + "\npan: " + ofToString(synth.getPan()) ;
	ofDrawBitmapString(tempStr, 50,ofGetHeight()-50);



	//---------------------------------- beats:
	if (beats.getIsPlaying()) ofSetHexColor(0xFF0000);
	else ofSetHexColor(0x000000);
	font.drawString("beats !!", widthDiv+50,50);

	ofSetHexColor(0x000000);
	tempStr = "click and drag\npct done: "+ofToString(beats.getPosition())+"\nspeed: " +ofToString(beats.getSpeed());
	ofDrawBitmapString(tempStr, widthDiv+50,ofGetHeight()-50);

	//---------------------------------- vocals:
	if (vocals.getIsPlaying()) ofSetHexColor(0xFF0000);
	else ofSetHexColor(0x000000);
	font.drawString("vocals !!", widthDiv*2+50,50);

	ofSetHexColor(0x000000);
	tempStr = "click to play (multiplay)\npct done: "+ofToString(vocals.getPosition())+"\nspeed: " + ofToString(vocals.getSpeed());	
	ofDrawBitmapString(tempStr, widthDiv*2+50,ofGetHeight()-50);


}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	// continuously control the speed of the beat sample via drag,
	// when in the "beat" region:
	float widthStep = ofGetWidth() / 3.0f;
	if (x >= widthStep && x < widthStep*2){
		beats.setSpeed( 0.5f + ((float)(ofGetHeight() - y) / (float)ofGetHeight())*1.0f);
	}

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	float widthStep = ofGetWidth() / 3.0f;
	if (x < widthStep){
		synth.play();
		synth.setSpeed( 0.1f + ((float)(ofGetHeight() - y) / (float)ofGetHeight())*10);
		synth.setPan(ofMap(x, 0, widthStep, -1, 1, true));
	} else if (x >= widthStep && x < widthStep*2){
		beats.play();
	} else {
		vocals.play();
		vocals.setSpeed( 0.1f + ((float)(ofGetHeight() - y) / (float)ofGetHeight())*3);
		//map x within the last third of window to -1 to 1 ( for left / right panning )
		vocals.setPan( ofMap(x, widthStep*2, widthStep*3, -1, 1, true) );
	}
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}
