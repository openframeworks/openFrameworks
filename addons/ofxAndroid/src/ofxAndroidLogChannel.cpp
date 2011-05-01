/*
 * ofxAndroidLogChannel.cpp
 *
 *  Created on: 20/01/2011
 *      Author: arturo
 */

#include "ofxAndroidLogChannel.h"

#include <android/log.h>

ofxAndroidLogChannel::ofxAndroidLogChannel(){

}

void ofxAndroidLogChannel::log(ofLogLevel level, const string & module, const string& msg){
	android_LogPriority androidPrio;
	switch (level){
	case OF_LOG_VERBOSE:
		androidPrio=ANDROID_LOG_VERBOSE;
		break;
	case OF_LOG_NOTICE:
		androidPrio=ANDROID_LOG_INFO;
		break;
	case OF_LOG_WARNING:
		androidPrio=ANDROID_LOG_WARN;
		break;
	case OF_LOG_ERROR:
		androidPrio=ANDROID_LOG_ERROR;
		break;
	case OF_LOG_FATAL_ERROR:
		androidPrio=ANDROID_LOG_FATAL;
		break;
	default:
		androidPrio=ANDROID_LOG_INFO;
		break;
	}
	__android_log_print(androidPrio,module.c_str(),msg.c_str());
}


void ofxAndroidLogChannel::log(ofLogLevel logLevel, const string & module, const char* format, ...){
	android_LogPriority androidPrio;
	switch (logLevel){
	case OF_LOG_VERBOSE:
		androidPrio=ANDROID_LOG_VERBOSE;
		break;
	case OF_LOG_NOTICE:
		androidPrio=ANDROID_LOG_INFO;
		break;
	case OF_LOG_WARNING:
		androidPrio=ANDROID_LOG_WARN;
		break;
	case OF_LOG_ERROR:
		androidPrio=ANDROID_LOG_ERROR;
		break;
	case OF_LOG_FATAL_ERROR:
		androidPrio=ANDROID_LOG_FATAL;
		break;
	default:
		androidPrio=ANDROID_LOG_INFO;
		break;
	}
	va_list args;
	va_start( args, format );
	__android_log_vprint(androidPrio,module.c_str(),format,args);
	va_end(args);
}

