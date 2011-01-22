#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	//ofSetDataPathRoot("./data");
	ofSetFrameRate(5);
	
	ofSetLogLevel(OF_LOG_ERROR);
	
	logTest();
	ofLogEnableHeader();
	logTest();
	ofLogNotice::setLevel(OF_LOG_WARNING);
	logTest();
	ofLogDisableHeader();
	
	ofLog(OF_LOG_NOTICE, "some more text here");
	ofLog(OF_LOG_WARNING, "%s %d", "text text text", 100);
	
	ofLogEnableFileRotationMins(1);
	ofLogEnableFile();
	ofLogEnableHeader();
	ofLogEnableHeaderFrameNum();
	ofLogEnableHeaderMillis();
	
	ofLogNotice::setLevel(OF_LOG_NOTICE);
	
	ofLogAddTopic("of.topic1");
	ofLogAddTopic("of.topic2");
	ofLogNotice("of") << "a log";
	ofLogNotice("of.topic1") << "an error";
	ofLogError("of.topic2") << "an error";
	ofLogSetTopicLogLevel("of.topic1", OF_LOG_WARNING);
	ofLogDebug("of.topic1") << "a log ... this should not be printed";
	ofLogError("of.topic1") << "an error";
	
	ofLogNotice() << "log file: " << ofLogGetFilePath();
	
	ofLogDisableHeaderDate();
	ofLogDisableHeaderTime();
	//ofLogdisableConsole();
}

//--------------------------------------------------------------
void testApp::update(){

	ofLogWarning() << "lalala";
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

void testApp::logTest(){
	ofLogNotice() << "log";
	ofLogDebug() << "debug";
	ofLogVerbose() << "verbose";
	ofLogWarning() << "warning";
	ofLogError() << "error";
	ofLogFatalError() << "fatal error";
}