#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
    ofBackground(255, 255, 255);
    
	ofSetFrameRate(60); // we set the frameRate slower, so that we see the movement of the lines better
	
	ofEnableAlphaBlending(); // this is necessary so that the colors can have alpha values
	
	// one way to define the fbo: setting the width (400), the height (200) and the internal format like this
	/*
	 ofFbo::Settings s;
	 s.width				= 400;
	 s.height			= 200;
	 s.internalformat    = GL_RGBA;
	 // and assigning this values to the fbo like this:
	 nonFloatingPointFbo_GL_RGB.allocate(s);
	 */
	
	// a second way of defining the fbo with the same values would be this:
	nonFloatingPointFbo_GL_RGB.allocate(400, 200, GL_RGB); // without alpha, 8 bits red, 8 bits green, 8 bits blue, from 0 to 255 in 256 steps
	
	
	// using the first way, we could have the same width and height for another fbo and just change the internal format like this:
	// s.internalformat = GL_RGBA;
	// and create the next fbo like this:
	// nonFloatingPointFbo_GL_RGBA.allocate(s);
	
	// or we can use the second way
	nonFloatingPointFbo_GL_RGBA.allocate(400, 200, GL_RGBA); // with alpha, 32 bits red, 32 bits green, 32 bits blue, from 0 to 1 in 'infinite' steps
	
    // here we create another two fbos with different internal formats using the second way:
	floatingPointFbo_GL_RGB32F_ARB.allocate(400, 200, GL_RGB32F_ARB); // without alpha
	//floatingPointFbo_GL_RGBA32F_ARB.allocate(400, 200, GL_RGBA32F_ARB); // with alpha
	
	floatingPointFbo_GL_RGBA32F_ARB.allocate(400, 200, GL_RGBA32F_ARB); // with alpha
	
    // we have to clear all the fbos so that we don't see any artefacts
	// the clearing color does not matter here, as the alpha value is 0, that means the fbo is cleared from all colors
	// whenever we want to draw/update something inside the fbo, we have to write that inbetween fbo.begin() and fbo.end()
	
    nonFloatingPointFbo_GL_RGB.begin();
	ofClear(255,255,255, 0); // alpha value 0
    nonFloatingPointFbo_GL_RGB.end();
    
    nonFloatingPointFbo_GL_RGBA.begin();
	ofClear(255,255,255, 0);
    nonFloatingPointFbo_GL_RGBA.end();
	
	floatingPointFbo_GL_RGB32F_ARB.begin();
	ofClear(255,255,255, 0);
    floatingPointFbo_GL_RGB32F_ARB.end();
	
	floatingPointFbo_GL_RGBA32F_ARB.begin();
	ofClear(255,255,255, 0);
    floatingPointFbo_GL_RGBA32F_ARB.end();
	
	
    
}

//--------------------------------------------------------------
void testApp::update(){

	// the background of the window is automatically cleared to BLACK every frame.
	
	// BEGIN FBO
	nonFloatingPointFbo_GL_RGB.begin();
	// we draw a blue rectangle with an alpha value of 3 into the fbo
	// the higher the alpha value, the sooner the white line disappears
	
	ofSetColor(0, 0 , 255, 3);
    ofRect(0,0,400,200); // the rectangle covers the whole area of the fbo with the size of 400 by 200
	
	// we draw a line that moves from the left to the right inside the fbo:
	ofSetColor(0,0,0);
	// ofGetFrameNum() counts up in integer numbers and with modulus % we always get numbers that count from 0 to 400
	// so that the x position of the line moves constantly from 0 to 400
	ofLine(ofGetFrameNum()% 401 , 0, ofGetFrameNum()% 401, 200);
	
    nonFloatingPointFbo_GL_RGB.end();
    // END FBO
	
	
	// we draw exactly the same ractangle and the same line into the other three fbos 
	
	
	// BEGIN FBO
    nonFloatingPointFbo_GL_RGBA.begin();
	
	ofSetColor(0, 0 , 255, 3);
    ofRect(0,0,400,200);
	
	ofSetColor(0,0,0);
	ofLine(ofGetFrameNum()% 401 , 0, ofGetFrameNum()% 401, 200);
	
	
	// what happens to the color in the non floating point fbo with GL_RGBA (with alpha):
	
	// the line is drawn with the color black, with an alpha value of 255			-> 0, 0, 0, 255
	// the blue rectangle is drawn with the color blue with an alpha value of 3		-> 0, 0, 255, 3
    
	
	
	
	
	
    nonFloatingPointFbo_GL_RGBA.end();
	// END FBO
	
	
	// BEGIN FBO
	floatingPointFbo_GL_RGB32F_ARB.begin();
	
	ofSetColor(0, 0 , 255, 3);
    ofRect(0,0,400,200);
	
	ofSetColor(0,0,0);
	ofLine(ofGetFrameNum()% 401 , 0, ofGetFrameNum()% 401, 200);
	
    floatingPointFbo_GL_RGB32F_ARB.end();
	// END FBO
	
	
	// BEGIN FBO
	floatingPointFbo_GL_RGBA32F_ARB.begin();
	
	ofSetColor(0, 0 , 255, 3);
    ofRect(0,0,400,200);
	
	ofSetColor(0,0,0);
	ofLine(ofGetFrameNum()% 401 , 0, ofGetFrameNum()% 401, 200);
	
    floatingPointFbo_GL_RGBA32F_ARB.end();
	// END FBO
	
	
}

//--------------------------------------------------------------
void testApp::draw(){
	
    // like an image the fbo also needs a color, if the color is set to white, the content of the fbo is drawn as it is
	ofSetColor(255, 255, 255);

	ofLine( 5, 110, ofGetWidth()-5, 110 );

	// the fbos have the size of 400px in width and 200px in height, so we draw them next to each other
	// the left top corner of the fbo is the point we have to write inside the draw function:
	nonFloatingPointFbo_GL_RGB.draw(10, 10); // 10px from the left, 10px from the top
	ofSetColor(0,0,0); // the font should be black
	ofDrawBitmapString("non floating point FBO: GL_RGB \n"
					   "without alpha", ofPoint(10, 230)); // subtitle
	
	ofSetColor(255, 255, 255);  // like an image the fbo also needs a color
    nonFloatingPointFbo_GL_RGBA.draw(420, 10); // 420px from the left, 10px from the top
	ofSetColor(0,0,0);
	ofDrawBitmapString("non floating point FBO: GL_RGBA \n"
					   "with alpha", ofPoint(420, 230)); // subtitle
	
	ofSetColor(255, 255, 255);
	ofLine( 5, 420, ofGetWidth()-5, 420 );
	
	floatingPointFbo_GL_RGB32F_ARB.draw(10, 310);
	ofSetColor(0,0,0);
	ofDrawBitmapString("floating point FBO: RGB32F_ARB \n"
					   "without alpha", ofPoint(10, 530)); // subtitle
	
	ofSetColor(255, 255, 255);
	floatingPointFbo_GL_RGBA32F_ARB.draw(420, 310);
	ofSetColor(0,0,0);
	ofDrawBitmapString("floating point FBO: GL_RGBA32F_ARB \n"
					   "with alpha", ofPoint(420, 530)); // subtitle
	
	
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