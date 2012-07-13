#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	sender.setup("localhost",6666);
	parameters.setName("parameters");
	parameters.add(size.set("size",10,1,100));
	parameters.add(number.set("number",10,1,100));
	colors.setName("colors");
	colors.add(r.set("r",127,0,255));
	colors.add(g.set("g",127,0,255));
	colors.add(b.set("b",127,0,255));
	colors.add(a.set("a",127,0,255));
	parameters.add(colors);
	gui.setup(parameters);
	ofSetVerticalSync(true);
}

//--------------------------------------------------------------
void testApp::update(){
	sender.sendParameter(parameters);
}

//--------------------------------------------------------------
void testApp::draw(){
	gui.draw();
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
