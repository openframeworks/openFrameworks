#pragma once

#include "ofConstants.h"

enum ofLogLevel{
	OF_LOG_VERBOSE,
	OF_LOG_NOTICE,
	OF_LOG_WARNING,
	OF_LOG_ERROR,
	OF_LOG_FATAL_ERROR,
	OF_LOG_SILENT	//this one is special and should always be last - set ofSetLogLevel to OF_SILENT to not recieve any messages
};

//NOTE: dan's logger is causing a ton of issues. 
//NOTE: reverting to the 0062 logger for now
//TODO: bring in his poco logger in a more managable way. 
//TODO: ofLogNotice << "something"; 

//--------------------------------------------------
void ofSetLogLevel(ofLogLevel logLevel);
void ofLog(ofLogLevel logLevel, string message);
void ofLog(ofLogLevel logLevel, const char* format, ...);
