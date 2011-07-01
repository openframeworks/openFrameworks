#include "testApp.h"
#include "ofGui.h"

bool bHide = true;

ofSlider radius;
ofSlider r;
ofSlider g;
ofSlider b;
ofSlider circleResolution;
ofToggle filled;

ofGuiCollection gui;

//--------------------------------------------------------------
void testApp::setup(){
	
	radius.setup( "radius", 140, 10, 300 );
	r.setup( "red", 100.0f, 0, 255 );
	g.setup( "green", 100.0f, 0, 255 );
	b.setup( "blue", 140.0f, 0, 255 );
	circleResolution.setup("circle res", 5, 3, 90, true);
	filled.setup("bFill", true);

	gui.setup("gui", 10, 10);
	gui.add(&filled);
	gui.add(&radius);
	gui.add(&r);
	gui.add(&g);
	gui.add(&b);
	gui.add(&circleResolution);
	
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
	ofCircle( ofGetWidth()/2, ofGetHeight()/2, radius.getValue() );

	if( bHide ){
		gui.draw();
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if( key == 'h' ){
		bHide = !bHide;
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