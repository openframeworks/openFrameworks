#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofDisableArbTex();
	if(ofIsGLProgrammableRenderer()){
		shaderBlurX.load("shadersGL3/shaderBlurX");
		shaderBlurY.load("shadersGL3/shaderBlurY");
	}else{
		shaderBlurX.load("shadersGL2/shaderBlurX");
		shaderBlurY.load("shadersGL2/shaderBlurY");
	}

	image.load("img.jpg");
	
	fboBlurOnePass.allocate(image.getWidth(), image.getHeight());
	fboBlurTwoPass.allocate(image.getWidth(), image.getHeight());
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	
	float blur = ofMap(mouseX, 0, ofGetWidth(), 0, 4, true);
	
	//----------------------------------------------------------
	fboBlurOnePass.begin();
	
	shaderBlurX.begin();
	shaderBlurX.setUniform1f("blurAmnt", blur);
	shaderBlurX.setUniform1f("texwidth", image.getWidth());

	image.draw(0, 0);
	
	shaderBlurX.end();
	
	fboBlurOnePass.end();
	
	//----------------------------------------------------------
	fboBlurTwoPass.begin();
	
	shaderBlurY.begin();
	shaderBlurY.setUniform1f("blurAmnt", blur);
	shaderBlurY.setUniform1f("texheight", image.getHeight());
	
	fboBlurOnePass.draw(0, 0);
	
	shaderBlurY.end();
	
	fboBlurTwoPass.end();
	
	//----------------------------------------------------------
	ofSetColor(ofColor::white);
	fboBlurTwoPass.draw(0, 0);
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
