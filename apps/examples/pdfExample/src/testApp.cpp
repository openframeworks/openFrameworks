#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	angle = 0;
	oneShot = false;
	pdfRendering = false;

	ofBackground(255,255,255);
	ofSetVerticalSync(true);
}

//--------------------------------------------------------------
void testApp::update(){
	angle++;
}

//--------------------------------------------------------------
void testApp::draw(){

	if( oneShot ){
		ofBeginSaveScreenAsPDF("screenshot-"+ofGetTimestampString()+".pdf", false);
	}
	
	ofSetColor(255,0,0);
	ofSetRectMode(OF_RECTMODE_CENTER);
	ofPushMatrix();

	float noise = ofNoise(float(ofGetFrameNum())/600.f);

	ofTranslate(noise*ofGetWidth(),noise*ofGetHeight());
	ofRotate(angle);
	ofTranslate(noise*-ofGetWidth(),noise*-ofGetHeight());

	ofRect(noise*ofGetWidth(),noise*ofGetHeight(),50,50);
	ofPopMatrix();

	ofSetColor(0,0,0);
	if( pdfRendering ){
		ofDrawBitmapString("press r to stop pdf multipage rendering",20,20);
	}else{	
		ofDrawBitmapString("press r to start pdf multipage rendering\npress s to save a single screenshot as pdf to disk",20,20);
	}
	
	if( oneShot ){
		ofEndSaveScreenAsPDF();
		oneShot = false;
	}	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	if( key=='r'){
		pdfRendering = !pdfRendering;	
		if( pdfRendering ){
			ofSetFrameRate(12);  // so it doesn't generate tons of pages
			ofBeginSaveScreenAsPDF("recording-"+ofGetTimestampString()+".pdf", true);
		}else{
			ofSetFrameRate(60);
			ofEndSaveScreenAsPDF();		
		}
	}
	
	if( !pdfRendering && key == 's' ){
		oneShot = true;
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
