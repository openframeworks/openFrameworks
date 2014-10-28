#include "ofApp.h"
#include "ofConstants.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);
	player.setPixelFormat(OF_PIXELS_RGBA);
	player.load("fingers.mov");
	bg.allocate(player.getPixels());
	pixels.allocate(player.getWidth(),player.getHeight());
	pixels.setUseTexture(false);
	thres.allocate(player.getWidth(),player.getHeight(),GL_R8);
	thres.setRGToRGBASwizzles(true);

	pixelsBuffer.allocate(pixels.getPixels(),GL_DYNAMIC_READ);
	shader.setupShaderFromFile(GL_COMPUTE_SHADER,"computeShader.glsl");
	shader.linkProgram();
	grabBG = true;
	player.play();

	player.getTexture().bindAsImage(0,GL_READ_ONLY);
	bg.bindAsImage(1,GL_READ_ONLY);
	thres.bindAsImage(2,GL_WRITE_ONLY);
}

//--------------------------------------------------------------
void ofApp::update(){
	player.update();
	if(player.isFrameNew()){
		if(grabBG){
			bg.loadData(player.getPixels());
			grabBG = false;
		}
		shader.begin();
		shader.setUniform1f("elapsedTime",ofGetElapsedTimef());
		shader.dispatchCompute(player.getWidth()/32, player.getHeight()/30, 1);
		shader.end();
		thres.copyTo(pixelsBuffer);
		unsigned char * p = pixelsBuffer.map<unsigned char>(GL_READ_ONLY);
		pixels.setFromPixels(p,player.getWidth(),player.getHeight());
		pixelsBuffer.unmap();
		contourFinder.findContours(pixels,10,player.getWidth()*player.getHeight()/3,10,false,false);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	player.draw(0,0);
	thres.draw(player.getWidth(),0);
	contourFinder.draw();
	ofDrawBitmapString(ofGetFrameRate(),20,20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key==' '){
		grabBG = true;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
