#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	
	
	
	
	

}

//--------------------------------------------------------------
void testApp::update(){

	if (loader.getStatus() == OF_URL_FILE_LOADER_LOADING_SUCCEEDED){
		printf("got an image -- %i bytes \n", loader.getData()->numBytes);
		loader.resetStatus();
	}
}

//--------------------------------------------------------------
void testApp::draw(){

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	loader.getBytes("http://i.cdn.turner.com/cnn/2011/OPINION/01/10/carter.sudan.future/tzleft.jimmy.carter.sudan.pres.talk.afpgi.jpg");

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

