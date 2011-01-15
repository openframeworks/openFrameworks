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
	temp.draw(dragPt.x, dragPt.y);
	
	ofSetColor(0);
	ofDrawBitmapString("drag images into this window", 10, 20);
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
void testApp::dragEvent(ofDragInfo info){
	
	if( info.files.size() > 0 ){
	
		dragPt = info.position;
		
		// try to load the first image. 
		temp.loadImage(info.files[0]);
		
		for (int i = 0; i < info.files.size(); i++){
			cout << info.files[i] << endl;
		}
		
		//printf("%s \n", fileNames[0].c_str());
	}

}

