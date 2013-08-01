#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	ofSetLogLevel(OF_LOG_VERBOSE);
	ofBackground(50, 50, 50);
	ofSetVerticalSync(false);
	ofEnableAlphaBlending();
	
	shader.setGeometryInputType(GL_LINES);
	shader.setGeometryOutputType(GL_TRIANGLE_STRIP);
	shader.setGeometryOutputCount(4);
	shader.load("shaders/vert.glsl", "shaders/frag.glsl", "shaders/geom.glsl"); 
	
	printf("Maximum number of output vertices support is: %i\n", shader.getGeometryMaxOutputCount());
	
	// create a bunch of random points
	float r = ofGetHeight()/2;
	for(int i=0; i<100; i++) {
		points.push_back(ofPoint(ofRandomf() * r, ofRandomf() * r, ofRandomf() * r));
	}
	
	doShader = true;	
	ofEnableDepthTest();
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

	ofColor(255);
	
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2, 0);
	ofRotateX(mouseY);
	ofRotateY(mouseX);

	for(unsigned int i=1; i<points.size(); i++) {
		ofLine(points[i-1], points[i]);
	}
	
	if(doShader) shader.end();
	
	ofPopMatrix();
	
	ofDrawBitmapString("fps: " + ofToString((int)ofGetFrameRate()) + "\nPress 's' to toggle shader: " + (doShader ? "ON" : "OFF"), 20, 20);
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	if( key == 's' ){
		doShader = !doShader;
	}	
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

