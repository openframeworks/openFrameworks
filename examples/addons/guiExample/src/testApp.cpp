#include "testApp.h"

	
//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	
	gui.setup("panel"); // most of the time you don't need a name
	gui.add(filled.setup("bFill", true));
	gui.add(radius.setup( "radius", 140, 10, 300 ));
	gui.add(r.setup( "red", 100.0f, 0, 255 ));
	gui.add(g.setup( "green", 100.0f, 0, 255 ));
	gui.add(b.setup( "blue", 140.0f, 0, 255 ));
	gui.add(circleResolution.setup("circle res", 5, 3, 90));
	gui.add(twoCircles.setup("twoCircles"));
	gui.add(ringButton.setup("ring"));
	
	ringButton.addListener(this,&testApp::ringButtonPressed);

	bHide = true;

	ring.loadSound("ring.wav");
}

void testApp::exit(){
	ringButton.removeListener(this,&testApp::ringButtonPressed);
}

void testApp::ringButtonPressed(bool & pressed){
	if(pressed) ring.play();
}

//--------------------------------------------------------------
void testApp::update(){
	ofSetCircleResolution(circleResolution);
}

//--------------------------------------------------------------
void testApp::draw(){

    ofBackgroundGradient(ofColor::white, ofColor::gray);
    
	if( filled ){
		ofFill();
	}else{
		ofNoFill();
	}

	ofSetColor(r, g, b);
	if(twoCircles){
		ofCircle(ofGetWidth()/2-radius*.5, ofGetHeight()/2, radius );
		ofCircle(ofGetWidth()/2+radius*.5, ofGetHeight()/2, radius );
	}else{
		ofCircle(ofGetWidth()/2, ofGetHeight()/2, radius );
	}
	
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
