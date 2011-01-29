/*
 * ofxAndroidLogChannel.cpp
 *
 *  Created on: 20/01/2011
 *      Author: arturo
 */

#include "ofxAndroidLogChannel.h"

#include <android/log.h>
#include "Poco/Message.h"

namespace Poco{
AndroidChannel::AndroidChannel(){

}


void AndroidChannel::log(const Message& msg){
	android_LogPriority androidPrio;
	switch (msg.getPriority()){
	case Message::PRIO_TRACE:
		androidPrio=ANDROID_LOG_VERBOSE;
		break;
	case Message::PRIO_DEBUG:
		androidPrio=ANDROID_LOG_DEBUG;
		break;
	case Message::PRIO_NOTICE:
		androidPrio=ANDROID_LOG_INFO;
		break;
	case Message::PRIO_WARNING:
		androidPrio=ANDROID_LOG_WARN;
		break;
	case Message::PRIO_ERROR:
		androidPrio=ANDROID_LOG_ERROR;
		break;
	case Message::PRIO_FATAL:
		androidPrio=ANDROID_LOG_FATAL;
		break;
	default:
		androidPrio=ANDROID_LOG_INFO;
		break;
	}
	__android_log_print(androidPrio,msg.getSource().c_str(),msg.getText().c_str());
}
}
