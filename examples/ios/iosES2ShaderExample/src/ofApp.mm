#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(40);
	ofSetVerticalSync(false);
	ofEnableAlphaBlending();
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    shader.load("shaders/noise.vert", "shaders/noise.frag");
    bUseShader = false;
    
	//we load a font and tell OF to make outlines so we can draw it as GL shapes rather than textures
    int fontSize = ofGetWidth() / 10;
	font.load("type/verdana.ttf", fontSize, true, false, true, 0.4, 72);
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofPushStyle();
	ofSetColor(245, 58, 135);
	ofFill();
    
    if(bUseShader) {
        shader.begin();
        shader.setUniform1f("timeValX", ofGetElapsedTimef() * 0.1 );     // we want to pass in some varrying values to animate our type / color
        shader.setUniform1f("timeValY", -ofGetElapsedTimef() * 0.18 );
        shader.setUniform2f("mouse", mousePoint.x, mousePoint.y);        // we also pass in the mouse position
    }
    
    ofRectangle rect = font.getStringBoundingBox("openFrameworks", 0, 0);   // size of text.
    int x = (ofGetWidth() - rect.width) * 0.5;                              // position in center screen.
    int padding = rect.height + 50;                                         // draw the text multiple times.
    for(int y=rect.height; y<ofGetHeight(); y+=padding) {
        font.drawStringAsShapes("openFrameworks", x, y);
    }

    if(bUseShader) {
        shader.end();
    }
    
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){
    bUseShader = true;
}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){
    // we have to transform the coords to what the shader is expecting which is 0,0 in the center and y axis flipped.
    mousePoint.x = touch.x * 2 - ofGetWidth();
    mousePoint.y = ofGetHeight() * 0.5 - touch.y;
}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch){
    bUseShader = false;
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

