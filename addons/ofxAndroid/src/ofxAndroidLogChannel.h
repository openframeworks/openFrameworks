/*
 * ofxAndroidLogChannel.h
 *
 *  Created on: 20/01/2011
 *      Author: arturo
 */

#pragma once
#include "Poco/Channel.h"

namespace Poco{
class AndroidChannel: public Channel{
public:
	AndroidChannel();

	void log(const Message& msg);
};
}

