/*
 * ofxAndroidLogChannel.cpp
 *
 *  Created on: 20/01/2011
 *      Author: arturo
 */

#include "ofxAndroidLogChannel.h"

#include <android/log.h>

using namespace std;

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
