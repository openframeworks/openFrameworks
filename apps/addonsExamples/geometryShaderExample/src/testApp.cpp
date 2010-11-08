#include "testApp.h"
#include "ofxShader.h"

// this examples uses a geometry shader to convert lines into triangle strips


ofxShader shader;
bool doShader = false;

vector<ofPoint> points;

//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofBackground(50, 50, 50);
	ofSetVerticalSync(false);
	ofEnableAlphaBlending();
	
	
	shader.setGeometryInputType(GL_LINES);
	shader.setGeometryOutputType(GL_TRIANGLE_STRIP);
	shader.setGeometryOutputCount(4);
	shader.setup("shaders/vert.glsl", "shaders/frag.glsl", "shaders/geom.glsl"); 
	
	printf("Maximum number of output vertices support is: %i\n", shader.getGeometryMaxOutputCount());
	
	// create a bunch of random points
	float r = ofGetHeight()/2;
	for(int i=0; i<100; i++) {
		points.push_back(ofPoint(ofRandomf() * r, ofRandomf() * r, ofRandomf() * r));
	}
	
	glEnable(GL_DEPTH_TEST);
}

//--------------------------------------------------------------
void testApp::update(){
}


//--------------------------------------------------------------
void testApp::draw(){
	ofPushMatrix();

	if(doShader) {
		shader.begin();
		
		// set thickness of ribbons
		shader.setUniform1f("thickness", 20);
		
		// make light direction slowly rotate
		shader.setUniform3f("lightDir", sin(ofGetElapsedTimef()/10), cos(ofGetElapsedTimef()/10), 0);
	}

	glColor3f(1, 1, 1);
	
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2, 0);
	ofRotateX(mouseY);
	ofRotateY(mouseX);

	for(int i=1; i<points.size(); i++) {
		ofLine(points[i-1], points[i]);
	}
	
	if(doShader) shader.end();
	
	ofPopMatrix();
	
	ofDrawBitmapString("fps: " + ofToString((int)ofGetFrameRate()) + "\nPress 's' to toggle shader: " + (doShader ? "ON" : "OFF"), 20, 20);
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	switch(key) {
		case 's':
			doShader ^= true;
			break;
	}
}

