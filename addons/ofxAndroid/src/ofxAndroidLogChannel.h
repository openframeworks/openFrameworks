/*
 * ofxAndroidLogChannel.h
 *
 *  Created on: 20/01/2011
 *      Author: arturo
 */

#pragma once
#include "ofLog.h"

class ofxAndroidLogChannel: public ofBaseLoggerChannel{
public:
	ofxAndroidLogChannel();

	void log(ofLogLevel level, const string & module, const string& msg);
	void log(ofLogLevel logLevel, const string & module, const char* format, ...);
	void log(ofLogLevel logLevel, const string & module, const char* format, va_list args);
};

