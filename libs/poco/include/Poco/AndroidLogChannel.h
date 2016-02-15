//
// AndroidLogChannel.h
//
// $Id: //poco/1.4/Foundation/include/Poco/AndroidLogChannel.h#2 $
//
// Library: Foundation
// Package: Logging
// Module:  AndroidLogChannel
//
// Definition of the AndroidLogChannel class.
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_AndroidLogChannel_INCLUDED
#define Foundation_AndroidLogChannel_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Channel.h"


namespace Poco {


class Foundation_API AndroidLogChannel: public Poco::Channel
	/// A channel that writes to the Android log subsystem.
	///
	/// Only the message's text is written, followed
	/// by a newline, using the tag passed on the constructor.
{
public:
	AndroidLogChannel(const std::string &tag);
		/// Creates the AndroidLogChannel.

	void log(const Message& msg);
		/// Logs the given message to the channel's stream.

protected:
	~AndroidLogChannel();

private:
	std::string _tag;
};


} // namespace Poco


#endif // Foundation_AndroidLogChannel_INCLUDED
