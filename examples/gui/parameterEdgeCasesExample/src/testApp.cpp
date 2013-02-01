#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	color.set("color",ofColor(127),ofColor(0,0),ofColor(255,255));
	color.addListener(this,&testApp::colorChanged);

	color = ofColor(255);
	cout << color.getName() << ": " << color << endl;

	// we can access the properties and const methods of objects contained
	// in ofParameters
	cout << color.getName() << ": r " << (int)color->r << ", g " << (int)color->g << ", b " << (int)color->b << endl;

	cout << "brightness: " << color->getBrightness() << endl;

	// but that's read only so always we change an object the internal event
	// in the ofParameter is triggered. Trying to do this, will fail to compile:

	//color->set(127);
	//color->a = 127;

	// instead we need to do:
	color = ofColor(127,127);
	cout << color.getName() << ": " << color << endl;
	cout << "brightness: " << color->getBrightness() << endl;

	// arithmetic operators will work though:
	cout << color.getName() << ": " << color << endl;
	color*=2;
	cout << color.getName() << ": " << color << endl;
}

void testApp::colorChanged(ofColor & color){
	cout << "colorChanged event: " << color << endl;
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
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
