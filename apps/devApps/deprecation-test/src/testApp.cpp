#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	OF_DEPRECATED_MSG("old string. Don't use", string oldstring);
//	(Un)comment these to trigger deprecation warnings
//	Note that only usage triggers warning, just declaration does not!
	deprecatedFunction();
	T1 t1object = 5;
	oldstring = "bla";
	oldvar3 = 5;
	oldClass bla;
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){

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

void testApp::deprecatedFunction(void){

}

