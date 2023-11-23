#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofDisableArbTex();
	if(ofIsGLProgrammableRenderer()){
		shader.load("shadersGL3/shader");
	}else{
		shader.load("shadersGL2/shader");
	}

	if(img.load("img.jpg")) {
		img.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
	}

	plane.set(800, 600, 10, 10);
	plane.mapTexCoords(0, 0, img.getWidth(), img.getHeight());
}


//--------------------------------------------------------------
void ofApp::update() {
	
}

//--------------------------------------------------------------
void ofApp::draw() {
	
	// bind our texture. in our shader this will now be tex0 by default
	// so we can just go ahead and access it there.
	img.getTexture().bind();
	
	// start our shader, in our OpenGL3 shader this will automagically set
	// up a lot of matrices that we want for figuring out the texture matrix
	// and the modelView matrix
	shader.begin();
	
	// get mouse position relative to center of screen
	float mousePosition = ofMap(mouseX, 0, ofGetWidth(), 1.0, -1.0, true);
	mousePosition *= plane.getWidth();

	shader.setUniform1f("mouseX", mousePosition);
	shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	ofPushMatrix();
	ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
	
	plane.draw();

	ofPopMatrix();
	
	shader.end();

	img.getTexture().unbind();
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
