#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	image.loadImage("F:/openFrameworks/examples/graphics/imageLoaderExample/bin/data/images/bikers.jpg");

	vector<string> sizeArray;
	sizeArray.push_back("hello");
	sizeArray.push_back("welcome");
	sizeArray.push_back("nice");

	gui.setup();
	testGroup.setup("testDropDownList");
	testGroup.add(testDropDownList.setup(sizeArray,200,18*sizeArray.size()));
	gui.add(&testGroup);

	gui.add(testContent.setup("imageTestConten","aaa",image,image.getWidth(),image.getHeight()));

	gui.add(testQuadWarp.setup("imageTestQuadWarp","bbb",image,image.getWidth(),image.getHeight()));

	gui.add(testToggle.setup("testToggle",false));
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
