/*
 
 Copyright 2007, 2008 Damian Stewart damian@frey.co.nz
 Distributed under the terms of the GNU Lesser General Public License v3
 
 This file is part of the ofxOsc openFrameworks OSC addon.
 
 ofxOsc is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 ofxOsc is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with ofxOsc.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef _ofxOscSENDER_H
#define _ofxOscSENDER_H

/**

ofxOscSender

an ofxOscSender sends messages to a single host/port

*/

class UdpTransmitSocket;
#include <string>
#include "OscTypes.h"
#include "OscOutboundPacketStream.h"

#include "ofxOscBundle.h"
#include "ofxOscMessage.h"


class ofxOscSender
{
public:
	ofxOscSender();
	~ofxOscSender();

	/// send messages to hostname and port
	void setup( std::string hostname, int port );

	/// send the given message
	void sendMessage( ofxOscMessage& message );
	/// send the given bundle
	void sendBundle( ofxOscBundle& bundle );

private:
		
	// helper methods for constructing messages
	void appendBundle( ofxOscBundle& bundle, osc::OutboundPacketStream& p );
	void appendMessage( ofxOscMessage& message, osc::OutboundPacketStream& p );

	UdpTransmitSocket* socket;
};

#endif
