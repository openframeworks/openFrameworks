#include "testApp.h"
#include "ofGui.h"

bool bHide = true;

ofSlider radius;
ofSlider r, g, b;
ofSlider circleResolution;
ofToggle filled;

ofPanel gui;
	
//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	
	gui.setup("panel"); // most of the time you don't need a name
	gui.add(filled.setup("bFill", true));
	gui.add(radius.setup( "radius", 140, 10, 300 ));
	gui.add(r.setup( "red", 100.0f, 0, 255 ));
	gui.add(g.setup( "green", 100.0f, 0, 255 ));
	gui.add(b.setup( "blue", 140.0f, 0, 255 ));
	gui.add(circleResolution.setup("circle res", 5, 3, 90, true));
	
}

//--------------------------------------------------------------
void testApp::update(){
	
}

//--------------------------------------------------------------
void testApp::draw(){
	
	if( filled.getValue() ){
		ofFill();
	}else{
		ofNoFill();
	}
	
	ofSetCircleResolution(circleResolution.getValue());
	ofSetColor(r.getValue(), g.getValue(), b.getValue());
	ofCircle(ofGetWidth()/2, ofGetHeight()/2, radius.getValue() );
	
	// auto draw?
	// should the gui control hiding?
	if( bHide ){
		gui.draw();
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if( key == 'h' ){
		bHide = !bHide;
	}
	if(key == 's') {
		gui.saveToFile("settings.xml");
	}
	if(key == 'l') {
		gui.loadFromFile("settings.xml");
	}
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