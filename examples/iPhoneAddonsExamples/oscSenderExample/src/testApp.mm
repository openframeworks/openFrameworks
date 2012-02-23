#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetOrientation(OF_ORIENTATION_90_LEFT);

	ofBackground( 40, 100, 40 );

	// open an outgoing connection to HOST:PORT
	sender.setup( HOST, PORT );
}

//--------------------------------------------------------------
void testApp::update(){
	//we do a heartbeat on iOS as the phone will shut down the network connection to save power
	//this keeps the network alive as it thinks it is being used. 
	if( ofGetFrameNum() % 120 == 0 ){
		ofxOscMessage m;
		m.setAddress( "/misc/heartbeat" );
		m.addIntArg( ofGetFrameNum() );
		sender.sendMessage( m );
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	// display instructions
	string buf;
	buf = "sending osc messages to" + string( HOST ) + ofToString( PORT );
	ofDrawBitmapString( buf, 10, 20 );
	ofDrawBitmapString( "move the mouse to send osc message [/mouse/position <x> <y>]", 10, 50 );
}

//--------------------------------------------------------------
void testApp::exit(){

}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs &touch){
	ofxOscMessage m;
	m.setAddress( "/mouse/button" );
	m.addStringArg( "down" );
	sender.sendMessage( m );
}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs &touch){
	ofxOscMessage m;
	m.setAddress( "/mouse/position" );
	m.addIntArg( touch.x );
	m.addIntArg( touch.y );
	sender.sendMessage( m );
}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs &touch){
	ofxOscMessage m;
	m.setAddress( "/mouse/button" );
	m.addStringArg( "up" );
	sender.sendMessage( m );
}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void testApp::lostFocus(){

}

//--------------------------------------------------------------
void testApp::gotFocus(){

}

//--------------------------------------------------------------
void testApp::gotMemoryWarning(){

}

//--------------------------------------------------------------
void testApp::deviceOrientationChanged(int newOrientation){

}


//--------------------------------------------------------------
void testApp::touchCancelled(ofTouchEventArgs& args){

}

