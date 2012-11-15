#include "testApp.h"
#include "ofGLES2Renderer.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(40);
	ofSetVerticalSync(false);
	ofEnableAlphaBlending();
    ofSetLogLevel(OF_LOG_VERBOSE);
    
	//we load a font and tell OF to make outlines so we can draw it as GL shapes rather than textures
    int fontSize = ofGetWidth() / 10;
	font.loadFont("type/verdana.ttf", fontSize, true, false, true, 0.4, 72);
}

//--------------------------------------------------------------
void testApp::update(){
    
}

//--------------------------------------------------------------
void testApp::draw(){
    ofPushStyle();
	ofSetColor(245, 58, 135);
	ofFill();
    
    ofShader & currentShader = ofGetGLES2Renderer()->getCurrentShader();
    currentShader.setUniform1f("timeValX", ofGetElapsedTimef() * 0.1 );     // we want to pass in some varrying values to animate our type / color
    currentShader.setUniform1f("timeValY", -ofGetElapsedTimef() * 0.18 );
    currentShader.setUniform2f("mouse", mousePoint.x, mousePoint.y);        // we also pass in the mouse position
    
    ofRectangle rect = font.getStringBoundingBox("openFrameworks", 0, 0);   // size of text.
    int x = (ofGetWidth() - rect.width) * 0.5;                              // position in center screen.
    int padding = rect.height + 50;                                         // draw the text multiple times.
    for(int y=rect.height; y<ofGetHeight(); y+=padding) {
        font.drawStringAsShapes("openFrameworks", x, y);
    }
    
    ofPopStyle();
}

//--------------------------------------------------------------
void testApp::exit(){

}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs & touch){
    //
}

//--------------------------------------------------------------
void mousePressed(int x, int y, int button);
    
    // we have to transform the coords to what the shader is expecting which is 0,0 in the center and y axis flipped.
    mousePoint.x = x * 2 - ofGetWidth();
    mousePoint.y = ofGetHeight() * 0.5 - y;
}
