#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    ofBackground(0,0,0);
	ofSetFrameRate(60);   
	
	//allocate our fbos. 
	//providing the dimensions and the format for the,
	rgbaFbo.allocate(400, 400, GL_RGBA); // with alpha, 8 bits red, 8 bits green, 8 bits blue, 8 bits alpha, from 0 to 255 in 256 steps	

	#ifdef TARGET_OPENGLES
	rgbaFboFloat.allocate(400, 400, GL_RGBA ); // with alpha, 32 bits red, 32 bits green, 32 bits blue, 32 bits alpha, from 0 to 1 in 'infinite' steps
        ofLogWarning("testApp") << "GL_RGBA32F_ARB is not available for OPENGLES.  Using RGBA.";	
	#else
        rgbaFboFloat.allocate(400, 400, GL_RGBA32F_ARB); // with alpha, 32 bits red, 32 bits green, 32 bits blue, 32 bits alpha, from 0 to 1 in 'infinite' steps
	#endif
	
	// we can also define the fbo with ofFbo::Settings.
	// this allows us so set more advanced options the width (400), the height (200) and the internal format like this
	/*
	 ofFbo::Settings s;
	 s.width			= 400;
	 s.height			= 200;
	 s.internalformat   = GL_RGBA;
	 s.useDepth			= true;
	 // and assigning this values to the fbo like this:
	 rgbFbo.allocate(s);
	 */
	 
		
    // we have to clear all the fbos so that we don't see any artefacts
	// the clearing color does not matter here, as the alpha value is 0, that means the fbo is cleared from all colors
	// whenever we want to draw/update something inside the fbo, we have to write that inbetween fbo.begin() and fbo.end()
    
    rgbaFbo.begin();
	ofClear(255,255,255, 0);
    rgbaFbo.end();
	
	rgbaFboFloat.begin();
	ofClear(255,255,255, 0);
    rgbaFboFloat.end();
    
}

//--------------------------------------------------------------
void testApp::update(){

    ofEnableAlphaBlending();
	
	//lets draw some graphics into our two fbos
    rgbaFbo.begin();
		drawFboTest();
    rgbaFbo.end();
	  	
    rgbaFboFloat.begin();
		drawFboTest();
	rgbaFboFloat.end();
	
}

//--------------------------------------------------------------
void testApp::drawFboTest(){
	//we clear the fbo if c is pressed. 
	//this completely clears the buffer so you won't see any trails
	if( ofGetKeyPressed('c') ){
		ofClear(255,255,255, 0);
	}	
	
	//some different alpha values for fading the fbo
	//the lower the number, the longer the trails will take to fade away.
	fadeAmnt = 40;
	if(ofGetKeyPressed('1')){
		fadeAmnt = 1;
	}else if(ofGetKeyPressed('2')){
		fadeAmnt = 5;
	}else if(ofGetKeyPressed('3')){
		fadeAmnt = 15;
	}  

	//1 - Fade Fbo
	
	//this is where we fade the fbo
	//by drawing a rectangle the size of the fbo with a small alpha value, we can slowly fade the current contents of the fbo. 
	ofFill();
	ofSetColor(255,255,255, fadeAmnt);
	ofRect(0,0,400,400);

	//2 - Draw graphics
	
	ofNoFill();
	ofSetColor(255,255,255);
	
	//we draw a cube in the center of the fbo and rotate it based on time
	ofPushMatrix();
		ofTranslate(200,200,0);
		ofRotate(ofGetElapsedTimef()*30, 1,0,0.5);
		ofDrawBox(0,0, 100);
	ofPopMatrix();

	//also draw based on our mouse position
	ofFill();	   
	ofCircle(ofGetMouseX() % 410, ofGetMouseY(), 8);

	//we move a line across the screen based on the time
	//the %400 makes the number stay in the 0-400 range. 
	int shiftX   = (ofGetElapsedTimeMillis() / 8 ) % 400;

	ofRect(shiftX, rgbaFbo.getHeight()-30, 3, 30);

}

//--------------------------------------------------------------
void testApp::draw(){
	
    ofSetColor(255,255,255);  	
    rgbaFbo.draw(0,0);
    rgbaFboFloat.draw(410,0);
	  	
    ofDrawBitmapString("non floating point FBO", ofPoint(10,20));
    ofDrawBitmapString("floating point FBO", ofPoint(420,20));

	string alphaInfo = "Current alpha fade amnt = " + ofToString(fadeAmnt);
	alphaInfo += "\nHold '1' to set alpha fade to 1";
	alphaInfo += "\nHold '2' to set alpha fade to 5";
	alphaInfo += "\nHold '3' to set alpha fade to 15";
	alphaInfo += "\nHold 'c' to clear the fbo each frame\n\nMove mouse to draw with a circle";
	
    ofDrawBitmapString(alphaInfo, ofPoint(10,430));
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
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
