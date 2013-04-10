#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
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
void testApp::dragEvent(ofDragInfo info){
	
	if( info.files.size() > 0 ){
		dragPt = info.position;
		
		draggedImages.assign( info.files.size(), ofImage() );
		for(unsigned int k = 0; k < info.files.size(); k++){
			draggedImages[k].loadImage(info.files[k]);
		}
	}

}

