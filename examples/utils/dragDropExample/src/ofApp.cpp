#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(255);
	
	float dx = dragPt.x;
	float dy = dragPt.y;
	for(unsigned int k = 0; k < draggedImages.size(); k++){
		draggedImages[k].draw(dx, dy);
		dy += draggedImages[k].getHeight() + 10;
	}
	
	ofSetColor(0);
	ofDrawBitmapString("drag image files into this window", 10, 20);
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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo info){
	
	if( info.files.size() > 0 ){
		dragPt = info.position;
		
		draggedImages.assign( info.files.size(), ofImage() );
		for(unsigned int k = 0; k < info.files.size(); k++){
			draggedImages[k].load(info.files[k]);
		}
	}

}

