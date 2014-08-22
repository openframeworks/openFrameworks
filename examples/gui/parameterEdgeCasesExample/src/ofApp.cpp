#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	color.set("color",ofColor(127),ofColor(0,0),ofColor(255,255));
	color.addListener(this,&ofApp::colorChanged);

	color = ofColor(255);
	cout << color.getName() << ": " << color << endl; // color: 255, 255, 255, 255

	// we can access the properties and const methods of objects contained
	// in ofParameters
	cout << color.getName() << ": r " << (int)color->r << ", g " << (int)color->g << ", b " << (int)color->b << endl;

	cout << "brightness: " << color->getBrightness() << endl; // brightness: 255

	// but that's read only so always we change an object the internal event
	// in the ofParameter is triggered. Trying to do this, will fail to compile:

	//color->set(127);
	//color->a = 127;

	// instead we need to do:
	color = ofColor(127,127);
	cout << color.getName() << ": " << color << endl; // color: 127, 127, 127, 127
	cout << "brightness: " << color->getBrightness() << endl; // brightness: 127

	// arithmetic operators will work though:
	cout << color.getName() << ": " << color << endl; // color: 127, 127, 127, 127
	color*=.5;
	cout << color.getName() << ": " << color << endl; // color: 255, 255, 255, 127




	// ofParameter has shallow copy semantics
	a.set("a",5,0,10);
	ofParameter<int> reference(a);

	cout << reference.getName() << ": " << reference << endl;  // a: 5

	incrementParam(reference);
	cout << reference.getName() << ": " << reference << endl;  // a: 6
	cout << a.getName() << ": " << a << endl;  				   // a: 6


	// to avoid confusion when doing assignments and arithmetic operations
	// the = operator only copies the value
	b.set("b",0,0,10);
	b = a;
	cout << b.getName() << ": " << b << endl;  // b: 6

	// if you want to make a reference:
	b.makeReferenceTo(a);
	cout << b.getName() << ": " << b << endl;  // a: 6

}

void ofApp::incrementParam(ofParameter<int> p){
	p++;
}

void ofApp::colorChanged(ofColor & color){
	cout << "colorChanged event: " << color << endl;
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){

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
