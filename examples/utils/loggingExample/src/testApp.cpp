#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetFrameRate(60);
	ofSetVerticalSync(true);

	startTime = ofGetElapsedTimeMillis();
	elapsedTime = 0;
	triggerOne = 0;
	triggerTwo = 0;

	verboseLevel = false;
	logToFile = false;

	// Bools for tracking changed conditions
	currentVerboseLevel = false;
	currentLogToFile = false;

	// Default log level is OF_NOTICE, so this doesn't have to be set
	// all log messages of level Notice and above will be shown.

	// We can set specific log levels for "modules", independent of the
	// global log level, like so:
	ofSetLogLevel("loggingChannelStuff", OF_LOG_VERBOSE);
}

//--------------------------------------------------------------
void testApp::update(){
	elapsedTime = ofGetElapsedTimeMillis() - startTime;

	if(elapsedTime >= triggerOne){
		ofLogVerbose() << "Run time is " << (elapsedTime / 1000) << " seconds"; //One way of composing log messages
		triggerOne += 1000; // triggerOne fires every second
	}

	if(elapsedTime >= triggerTwo){
		ofLog(OF_LOG_NOTICE, "5s timer: Run time is %i seconds", elapsedTime / 1000); //Another way of composing log messages
		triggerTwo += 5000; // triggerTwo fires every 5 seconds
	}

	if(verboseLevel != currentVerboseLevel){ // if the verbosity level has just changed
		currentVerboseLevel = verboseLevel;
		if(verboseLevel){ // set log level according to button state
			ofSetLogLevel(OF_LOG_VERBOSE);
		}
		else{
			ofSetLogLevel(OF_LOG_NOTICE);
		}
		ofLog(OF_LOG_FATAL_ERROR, "New log level is " + ofGetLogLevelName(ofGetLogLevel())); // The third way of composing log messages
	}

	if(logToFile != currentLogToFile){ // if the logging channel has just changed
		currentLogToFile = logToFile;
		if(logToFile){
			// Module-specific logging
			// These print always, even if global log level is Notice
			// They also start with the module name
			ofLogVerbose("loggingChannelStuff") << "Switching to file logging";
			ofLogToFile("logfile.log", true); //set channel to log file. log file resides in the /bin/data folder
			ofLogVerbose("loggingChannelStuff") << "Logging to file now";
		}
		else{
			ofLogVerbose("loggingChannelStuff") << "Switching to console logging";
			ofLogToConsole(); //set channel to console
			ofLogVerbose("loggingChannelStuff") << "Logging to console now";
		}
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	
	string verboseStr = "[ ]";
	if( verboseLevel ) verboseStr = "[x]";

	string logStr = "[ ]";
	if( logToFile ) logStr = "[x]";
	
	ofSetColor(20);
	ofDrawBitmapString("Key Controls:\nv: Set verbose log "+verboseStr+"\nf: Set log to file "+logStr+"\n ", 20, 30);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if( key == 'v' ){
		verboseLevel = !verboseLevel;
	}
	if( key == 'f' ){
		logToFile = !logToFile;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	// logging message triggered by event
	ofLog(OF_LOG_WARNING, "Mouse button %i was pressed at x=%i, y=%i !", button, x, y);
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
