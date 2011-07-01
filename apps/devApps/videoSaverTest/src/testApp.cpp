#include "testApp.h"


static bool bRecording = false;



//--------------------------------------------------------------
void testApp::setup(){

	camWidth 		= 320;	// try to grab at this size. 
	camHeight 		= 240;
	
	vidGrabber.initGrabber(camWidth,camHeight);
	vidGrabber.setVerbose(true);
	
	
	
	saver.listCodecs();
	//saver.setCodecType(2); ZACH FIX see recording quicktime with sound in sync on of forum
	saver.setCodecQualityLevel(OF_QT_SAVER_CODEC_QUALITY_NORMAL);
	saver.setup(320,240,"output.mov");
	
	ofSetFrameRate(30);
	
	
}

//--------------------------------------------------------------
void testApp::update(){

	ofBackground(100,100,100);
	
	vidGrabber.grabFrame();
	
	
	if (bRecording == true){
		saver.addFrame(vidGrabber.getPixels(), 1.0f / 30.0f); 
	}
	
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(255,255,255);
	vidGrabber.draw(20,20);
	
	if (saver.bAmSetupForRecording()){
		ofSetColor(255,255,255);
		ofDrawBitmapString("setup for recording, (press mouse to record / mouse up to stop)",100,300);
		
		if (bRecording){
			ofSetColor(0,255,0);
			ofDrawBitmapString("recording",100,320);
		} else {
			ofSetColor(255,255,255);
			ofDrawBitmapString("not recording",100,320);
		}
	} 
	
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

static int counter = 0;
//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	saver.setup(320,240,"output_" +  ofToString(counter++) + ".mov");
	bRecording = true;
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	bRecording = false;
	saver.finishMovie();

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

