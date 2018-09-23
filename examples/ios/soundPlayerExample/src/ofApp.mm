#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofBackground(255);
	ofSetOrientation(OF_ORIENTATION_90_RIGHT);

	synth.load("sounds/synth.mp3"); // compressed mp3 format.
    synth.setVolume(0.75);

	beats.load("sounds/1085.caf"); // uncompressed caf format.
    beats.setVolume(0.75);

    // in iOS, openFrameworks uses ofxiOSSoundPlayer to play sound.
    // ofxiOSSoundPlayer is a wrapper for AVSoundPlayer which uses AVFoundation to play sound.
    // you can use AVSoundPlayer directly using objective-c in the same way you use ofxiOSSoundPlayer,
    // and many of the function names are the same or similar.
    // the below code demonstrates how the AVSoundPlayer can be used inside your app.

    vocals = [[AVSoundPlayer alloc] init];
    [vocals loadWithFile:@"sounds/Violet.wav"]; // uncompressed wav format.
    [vocals volume:0.5];

    font.load("fonts/Sudbury_Basin_3D.ttf", 18);
}

//--------------------------------------------------------------
void ofApp::update(){
	//
}

//--------------------------------------------------------------
void ofApp::draw(){

    string tempStr;

	float sectionWidth = ofGetWidth() / 3.0f;

    // draw the background colors:
	ofSetHexColor(0xeeeeee);
	ofDrawRectangle(0, 0, sectionWidth, ofGetHeight());
	ofSetHexColor(0xffffff);
	ofDrawRectangle(sectionWidth, 0, sectionWidth, ofGetHeight());
	ofSetHexColor(0xdddddd);
	ofDrawRectangle(sectionWidth * 2, 0, sectionWidth, ofGetHeight());

	//---------------------------------- synth:
	if(synth.isPlaying()) {
        ofSetHexColor(0xFF0000);
    } else {
        ofSetHexColor(0x000000);
    }
	font.drawString("synth !!", 10,50);

	ofSetColor(0);
    tempStr = "click to play\nposition: " + ofToString(synth.getPosition()) + "\nspeed: " + ofToString(synth.getSpeed()) + "\npan:" + ofToString(synth.getPan());
	ofDrawBitmapString(tempStr, 10, ofGetHeight() - 50);

	//---------------------------------- beats:
	if (beats.isPlaying()) {
        ofSetHexColor(0xFF0000);
    } else {
        ofSetHexColor(0x000000);
    }
	font.drawString("beats !!", sectionWidth + 10, 50);

	ofSetHexColor(0x000000);
    tempStr = "click to play\nposition: " + ofToString(beats.getPosition()) + "\nspeed: " + ofToString(beats.getSpeed()) + "\npan: " + ofToString(beats.getPan());
	ofDrawBitmapString(tempStr, sectionWidth + 10, ofGetHeight() - 50);

	//---------------------------------- vocals:
	if ([vocals isPlaying]) {
        ofSetHexColor(0xFF0000);
    } else {
        ofSetHexColor(0x000000);
    }
	font.drawString("vocals !!", sectionWidth * 2 + 10, 50);

	ofSetHexColor(0x000000);
    tempStr = "click to play\nposition: " + ofToString(vocals.position) + "\nspeed: " + ofToString(vocals.speed) + "\npan: " + ofToString(vocals.pan);
	ofDrawBitmapString(tempStr, sectionWidth * 2 + 10, ofGetHeight() - 50);
}

//--------------------------------------------------------------
void ofApp::exit(){
    [vocals release];
    vocals = nil;
}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){
	if( touch.id != 0 ){
        return;
    }

    float sectionWidth = ofGetWidth() / 3.0f;
    float speed = ofMap(touch.y, ofGetHeight(), 0, 0.5, 2.0, true);
    float pan = 0;

    if (touch.x < sectionWidth){
        pan = ofMap(touch.x, 0, sectionWidth, -1.0, 1.0, true);

        synth.play();
        synth.setSpeed(speed);
        synth.setPan(pan);

    } else if(touch.x < sectionWidth * 2) {
        pan = ofMap(touch.x, sectionWidth, sectionWidth * 2, -1.0, 1.0, true);

        beats.play();
        beats.setSpeed(speed);
        beats.setPan(pan);

    } else if(touch.x < sectionWidth * 3) {
        pan = ofMap(touch.x, sectionWidth * 2, sectionWidth * 3, -1.0, 1.0, true);

        [vocals play];
        [vocals speed:speed];
        [vocals pan:pan];
    }
}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){
	if( touch.id != 0 ){
        return;
    }

    float sectionWidth = ofGetWidth() / 3.0f;
    float speed = ofMap(touch.y, ofGetHeight(), 0, 0.5, 2.0, true);
    float pan = 0;

    if (touch.x < sectionWidth){
        pan = ofMap(touch.x, 0, sectionWidth, -1.0, 1.0, true);

        synth.setSpeed(speed);
        synth.setPan(pan);

    } else if(touch.x < sectionWidth * 2) {
        pan = ofMap(touch.x, sectionWidth, sectionWidth * 2, -1.0, 1.0, true);

        beats.setSpeed(speed);
        beats.setPan(pan);

    } else if(touch.x < sectionWidth * 3) {
        pan = ofMap(touch.x, sectionWidth * 2, sectionWidth * 3, -1.0, 1.0, true);

        [vocals speed:speed];
        [vocals pan:pan];
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
