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
	floatingPointFbo_GL_RGBA32F_ARB.allocate(400, 200, GL_RGBA32F_ARB); // with alpha
	
    // we have to clear all the fbos so that we don't see any artefacts
	// the clearing color does not matter here, as the alpha value is 0, that means the fbo is cleared from all colors
	// whenever we want to draw/update something inside the fbo, we have to write that inbetween fbo.begin() and fbo.end()
	
    nonFloatingPointFbo_GL_RGB.begin();
	ofClear(255,255,255, 0); // alpha value 0, the fbo becomes transparent
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
	
	// the background of the window is automatically cleared to the background color every frame.
	
	// BEGIN FBO
	nonFloatingPointFbo_GL_RGB.begin();
	// we draw a blue rectangle with an alpha value of 5 into the fbo
	// the higher the alpha value, the sooner the white line disappears
	
	ofSetColor(0, 0 , 255, 5);
    ofRect(0,0,400,200); // the rectangle covers the whole area of the fbo with the size of 400 by 200
	
	// we draw a line that moves from the left to the right inside the fbo:
	ofSetColor(0,0,0);
	// ofGetFrameNum() counts up in integer numbers and with modulus % we always get numbers that count from 0 to 400
	// so that the x position of the line moves constantly from 0 to 400
	ofLine(ofGetFrameNum()% 401 , 0, ofGetFrameNum()% 401, 200);
	
	
	// what happens inside the fbo here ?:
	
	// the black line that is drawn first on the very left side of the fbo gets more and more blue 
	// and it also gets more and more transparent over time.
	// the color changes from 0,0,0 to 0,0,255 
	// and the alpha value changes from 255 (fully visible) to 5 (very transparent).
	// after enough frames the black line which was drawn first on the very left with an alpha of 255 becomes blue with an alpha of 5
	// when the fbo is drawn onto a white background, the parts that are transparent appear to be more white
	
	// oldColor: black, alpha 255, newColor: blue, alpha 5
	// each frame the color gets more blue: oldColor * (255 - alpha) + (newColor * alpha)
	
	
    nonFloatingPointFbo_GL_RGB.end();
    // END FBO
	
	
	
	// we draw exactly the same ractangle and the same moving line into the other three fbos 
	
	
	// BEGIN FBO
    nonFloatingPointFbo_GL_RGBA.begin();
	
	ofSetColor(0, 0 , 255, 5);
    ofRect(0,0,400,200);
	
	ofSetColor(0,0,0);
	ofLine(ofGetFrameNum()% 401 , 0, ofGetFrameNum()% 401, 200);
	
	// what happens to the color in the non floating point fbo with GL_RGBA (with alpha):
	// the line is drawn with the color black, with an alpha value of 255			-> 0, 0, 0, 255
	// the blue rectangle is drawn with the color blue with an alpha value of 5		-> 0, 0, 255, 5
	// the color on the very left side of the fbo gets more blue and more transparent over time, 
	// but because of a rounding error, it does not reach the same numbers (in color and alpha) as in the floating point fbo.
    
    nonFloatingPointFbo_GL_RGBA.end();
	// END FBO
	
	
	// BEGIN FBO
	floatingPointFbo_GL_RGB32F_ARB.begin();
	
	ofSetColor(0, 0 , 255, 5);
    ofRect(0,0,400,200);
	
	ofSetColor(0,0,0);
	ofLine(ofGetFrameNum()% 401 , 0, ofGetFrameNum()% 401, 200);
	
    floatingPointFbo_GL_RGB32F_ARB.end();
	// END FBO
	
	
	// BEGIN FBO
	floatingPointFbo_GL_RGBA32F_ARB.begin();
	
	ofSetColor(0, 0 , 255, 5);
    ofRect(0,0,400,200);
	
	ofSetColor(0,0,0);
	ofLine(ofGetFrameNum()% 401 , 0, ofGetFrameNum()% 401, 200);
	
    floatingPointFbo_GL_RGBA32F_ARB.end();
	// END FBO
	
	
}

//--------------------------------------------------------------
void testApp::draw(){
	
	
	
	
    // horizontal black line to see the difference between drawing in the fbo and drawing on the screen 
	ofSetColor(0, 0, 0);
	ofLine( 5, 110, ofGetWidth()-5, 110 ); // this line is drawn directly onto the screen, not inside the fbo
	
	
	
	
	// FBO on the top left:
	// similar to an ofImage the fbo also needs a color set before it is drawn, if the color is set to white, the content of the fbo is drawn as it is
	ofSetColor(255, 255, 255);
	// the fbos have the size of 400px in width and 200px in height, so we draw them next to each other
	// the left top corner of the fbo is the point we have to write inside the draw function to position it on the screen
	nonFloatingPointFbo_GL_RGB.draw(10, 10); // 10px from the left, 10px from the top
	
	
	// the following lines of code are just for reading the color on the SCREEN where the top left of the fbo is located
	// because we want to write these values directly under the fbo as a subtitle onto the screen
	ofImage screenIMG; // we crate an image object
	screenIMG.allocate( 1, 1, OF_IMAGE_COLOR ); // we say that the image object should be 1px by 1px
	screenIMG.grabScreen( 10, 10, 1, 1 ); // we grab the color in the rectangle with a width of 1 and a height of 1 at the screen location 10, 10 
	ofColor c = screenIMG.getColor( 0, 0); // this color is assigned to the color variable c
	
	// SUBTITLE for the fbo on the top left 
	ofSetColor(0,0,0); // the font should be black
	ofDrawBitmapString("non floating point FBO: GL_RGB - without alpha \n"
					   "\n"
					   "color of the screen on the top left of the fbo: \n"
					   "red:" + ofToString(int(c.r)) + " green:" + ofToString(int(c.g)) + " blue:" + ofToString(int(c.b)), 
					   ofPoint(10, 230)); // subtitle
	
	
	
	
	
	// FBO on the top right:
	ofSetColor(255, 255, 255);  // "color" for the fbo
    nonFloatingPointFbo_GL_RGBA.draw(420, 10); // 420px from the left, 10px from the top
	
	// the following lines of code are just for reading the color on the SCREEN where the top left of the fbo is located
	// because we want to write these values directly under the fbo as a subtitle onto the screen
	screenIMG.grabScreen( 430, 10, 1, 1 ); // grab the color of one pixel in the upper left of the fbo
	c = screenIMG.getColor(0, 0);
	
	// the following lines of code are just for reading the color inside the FBO
	// because we want to write these values directly under the fbo as a subtitle onto the screen
	ofFloatPixels pixels;
	nonFloatingPointFbo_GL_RGBA.readToPixels( pixels );
	ofColor fboColor = pixels.getColor( 0, 0 ); // we read the color at the very top left inside the fbo
	
	// SUBTITLE for the fbo on the top right
	ofSetColor(0,0,0); // black font
	ofDrawBitmapString("non floating point FBO: GL_RGBA - with alpha \n"
					   "\n"
					   "color of the screen on the top left of the fbo: \n"
					   "red:" + ofToString(int(c.r)) + " green:" + ofToString(int(c.g)) + " blue:" + ofToString(int(c.b)) + "\n"
					   "\n"
					   "color of the fbo on the top left of the fbo: \n"
					   "red:" + ofToString(int(fboColor.r)) + " green:" + ofToString(int(fboColor.g)) + " blue:" + 
					   ofToString(int(fboColor.b)) + " alpha:" + ofToString(int(fboColor.a)), ofPoint(420, 230)); // subtitle 
	
	
	
	
	
	
	// second horizontal black line to see the difference between drawing in the fbo and drawing on the screen 
	ofSetColor(0, 0, 0);
	ofLine( 5, 420, ofGetWidth()-5, 420 );
	
	
	
	
	
	// FBO on the bottom left:
	ofSetColor(255, 255, 255);
	floatingPointFbo_GL_RGB32F_ARB.draw(10, 330);
	
	// the following lines of code are just for reading the color on the SCREEN where the top left of the fbo is located
	// because we want to write these values directly under the fbo as a subtitle onto the screen
	screenIMG.grabScreen( 10, 330, 1, 1 ); // grab the color of one pixel in the upper left of the fbo
	c = screenIMG.getColor(0, 0);
	
	// SUBTITLE for the fbo on the bottom left
	ofSetColor(0,0,0);
	ofDrawBitmapString("floating point FBO: RGB32F_ARB - without alpha \n"
					   "\n"
					   "color of the screen on the top left of the fbo: \n"
					   "red:" + ofToString(float(c.r)) + " green:" + ofToString(float(c.g)) + " blue:" + ofToString(float(c.b)),
					   ofPoint(10, 550));
	
	
	
	
	
	// FBO on the bottom right:
	
	ofSetColor(255, 255, 255);
	floatingPointFbo_GL_RGBA32F_ARB.draw(420, 330);
	
	// the following lines of code are just for reading the color on the SCREEN where the top left of the fbo is located
	// because we want to write these values directly under the fbo as a subtitle onto the screen
	screenIMG.grabScreen( 420, 330, 1, 1 ); // grab the color of one pixel in the upper left of the fbo
	c = screenIMG.getColor(0, 0);
	
	// the following lines of code are just for reading the color inside the FBO
	// because we want to write these values directly under the fbo as a subtitle onto the screen
	floatingPointFbo_GL_RGBA32F_ARB.readToPixels( pixels );
	ofFloatColor fboFloatColor = pixels.getColor( 0, 0 ); // we read the color at the very top left inside the fbo
	
	// SUBTITLE for the fbo on the bottom right
	ofSetColor(0,0,0);
	ofDrawBitmapString("floating point FBO: GL_RGBA32F_ARB - with alpha \n"
					   "\n"
					   "color of the screen on the top left of the fbo: \n"
					   "red:" + ofToString(float(c.r)) + " green:" + ofToString(float(c.g)) + " blue:" + ofToString(float(c.b)) + "\n"
					   "\n"
					   "color of the fbo on the top left of the fbo: \n"
					   "red:" + ofToString(float(fboFloatColor.r)) + " green:" + ofToString(float(fboFloatColor.g)) + " blue:" 
					   + ofToString(float(fboFloatColor.b), 6) + " alpha:" + ofToString(float(fboFloatColor.a), 6), ofPoint(420, 550)); 
	
	
	
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