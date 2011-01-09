#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	temp.draw(0,0);
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

void testApp::dragEvent(vector < string > fileNames){
	
	if (fileNames.size() > 0){
		
		// try to load the first image. 
		temp.loadImage( fileNames[0]);
		
		for (int i = 0; i < fileNames.size(); i++){
			cout << fileNames[i] << endl;
		}
		
		//printf("%s \n", fileNames[0].c_str());
	}
	
	
}

