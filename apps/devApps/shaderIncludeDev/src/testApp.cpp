#include "testApp.h"

ofShader shader;
ofImage img;
//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	img.loadImage("testImage.jpg");
	shader.load("", "shaders/test.frag");
	
	ofBackground(40, 40, 40);
}

//--------------------------------------------------------------
void testApp::update(){
	
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(255);
	shader.begin();
		shader.setUniform1i("tex0", 0);
		shader.setUniform1f("blurScale", ofGetMouseX() / 600.0 );  
		shader.setUniform1f("saturation", ofMap(ofGetMouseY(), 0, ofGetHeight(), 0.3, 2.0, true));  
		img.draw(0,0);
	shader.end(); 
	
	ofDrawBitmapString("mouse x - controls blur amount\nmouse y - controls saturation", 10, img.getHeight()+20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

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