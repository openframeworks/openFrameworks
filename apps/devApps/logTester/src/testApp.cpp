#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	ofSetFrameRate(5);
	
	ofSetLogLevel(OF_LOG_VERBOSE);
	logTest("log level:OF_LOG_VERBOSE");
	
	ofSetLogLevel(OF_LOG_NOTICE);
	logTest("log level:OF_LOG_NOTICE");
	
	ofSetLogLevel(OF_LOG_WARNING);
	logTest("log level:OF_LOG_WARNING");
	
	ofSetLogLevel(OF_LOG_ERROR);
	logTest("log level:OF_LOG_ERROR");
	
	ofSetLogLevel(OF_LOG_FATAL_ERROR);
	logTest("log level:OF_LOG_FATAL_ERROR");
	
	ofSetLogLevel(OF_LOG_NOTICE);
}

//--------------------------------------------------------------
void testApp::update(){

	ofLogWarning() << "frame: " << ofGetFrameNum()
				   << "  fps: " << ofGetFrameRate();
}

//--------------------------------------------------------------
void testApp::draw(){
	
	//this is not part of ofLog - needs to be discussed before we put this on
	//ie - is this less effecient than using a global function. 
//	ofDrawBitmapString(20, 12) << "frame: " << ofGetFrameNum() << endl
//							   << "  fps: " << ofGetFrameRate();

	ofDrawBitmapString("millis: "+ofToString(ofGetElapsedTimeMillis()), 20, 100);
}

//--------------------------------------------------------------
void testApp::logTest(const string& msg){
	cout << msg << endl; 
	ofLog() << "log";
	ofLogVerbose() << "verbose";
	ofLogWarning() << "warning";
	ofLogError() << "error";
	ofLogFatalError() << "fatal error";
	ofLog(OF_LOG_NOTICE, "some more text here");
	ofLog(OF_LOG_ERROR, "%s %d", "text text text", 100);
	cout << endl; 
}