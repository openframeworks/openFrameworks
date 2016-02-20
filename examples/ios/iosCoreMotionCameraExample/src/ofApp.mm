#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	

    ofSetOrientation(OF_ORIENTATION_90_LEFT);
    ofSetFrameRate(60);
    ofBackground(255, 255, 0);
    
    camera.setupPerspective();
    camera.setVFlip(false);
    camera.setPosition(0, 0, 0);

    coreMotion.setupAttitude(CMAttitudeReferenceFrameXMagneticNorthZVertical);
}

//--------------------------------------------------------------
void ofApp::update(){

    coreMotion.update();
    
    // attitude- quaternion
    ofQuaternion quat = coreMotion.getQuaternion();
    ofQuaternion landscapeFix(-quat.y(), quat.x(), quat.z(), quat.w());
    camera.setOrientation(landscapeFix);
}

//--------------------------------------------------------------
void ofApp::draw(){

    camera.begin();
    
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    ofNoFill();
    ofSetColor(0);
	ofDrawBox(0, 0, 0, 1000);
    ofDrawAxis(100);
    ofPopMatrix();
    
    camera.end();
}

//--------------------------------------------------------------
void ofApp::exit(){

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
