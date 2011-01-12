#include "testApp.h"
#include "demo1.h"
#include "demo2.h"
#include "demo3.h"
#include "demo4.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofBackground(200, 200, 200);
	ofSetVerticalSync(true);
	
	useMSAA = false;
	
	setupDemo(1);
	
	ofEnableAlphaBlending();
	
	vidGrabber.initGrabber(320, 240);
}

//--------------------------------------------------------------
void testApp::setupDemo(int i) {
	demoMode = i;
	switch(demoMode) {
		case 1: demo1_setup(); break;
		case 2: demo2_setup(); break;
		case 3: demo3_setup(); break;
		case 4: demo4_setup(); break;
	}
}

//--------------------------------------------------------------
void testApp::update(){
	vidGrabber.update();
}

//--------------------------------------------------------------
void testApp::drawScene(float x, float y, float w, float h) {
	glColor3f(1, 1, 1);
	vidGrabber.draw(x, y, w, h);
	
	
	glColor3f(1, 0, 0);
	ofCircle(x + w * (0.5 + 0.5 * sin(ofGetElapsedTimef())), y + h * (0.5 + 0.5 * cos(ofGetElapsedTimef())), 10);
	
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(w/1024.0, h/768.0, 1);		// coordinates below were for 1024x768 screen
	
	static float counter = 0;

	//--------------------------- circles
	//let's draw a circle:
	ofSetColor(255,130,0);
	float radius = 50 + 10 * sin(counter);
	ofFill();		// draw "filled shapes"
	ofCircle(100,400,radius);
	
	// now just an outline
	ofNoFill();
	ofSetHexColor(0xCCCCCC);
	ofCircle(100,400,80);
	
	//--------------------------- rectangles
	ofFill();
	for (int i = 0; i < 200; i++){
		ofSetColor((int)ofRandom(0,255),(int)ofRandom(0,255),(int)ofRandom(0,255));
		ofRect(ofRandom(250,350),ofRandom(350,450),ofRandom(10,20),ofRandom(10,20));
	}
	
	//---------------------------  transparency
	ofSetHexColor(0x00FF33);
	ofRect(400,350,100,100);
	ofSetColor(255,0,0,127);   // red, 50% transparent
	ofRect(450,430,100,33);
	ofSetColor(255,0,0,(int)(counter * 10.0f) % 255);   // red, variable transparent
	ofRect(450,370,100,33);
	
	//---------------------------  lines
	ofSetHexColor(0xFF0000);
	for (int i = 0; i < 20; i++){
		ofLine(600,300 + (i*5),800, 250 + (i*10));
	}
	
	glPopMatrix();
	
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void testApp::draw(){
	// draw original to screen to compare
	drawScene(0, 0, kPreviewWidth, kPreviewHeight);

	glColor3f(0, 0, 0);
	ofDrawBitmapString("Original", kLabelX(0), kLabelY(0));
	
	switch(demoMode) {
		case 1: demo1_draw(); break;
		case 2: demo2_draw(); break;
		case 3: demo3_draw(); break;
		case 4: demo4_draw(); break;
	}
	
	glColor3f(0, 0, 0);
	ofDrawBitmapString("fps: " + ofToString((int)ofGetFrameRate()), ofGetWidth() - 100, 20);

}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	switch(key) {
		case '1': setupDemo(1); break;
		case '2': setupDemo(2); break;
		case '3': setupDemo(3); break;
		case '4': setupDemo(4); break;
			
		case 'm': useMSAA ^= true; setupDemo(demoMode); break;
	}
			
}

