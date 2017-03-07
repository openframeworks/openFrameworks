/*
 
 Copyright (c) 2007-2009, Damian Stewart
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 * Neither the name of the developer nor the
 names of its contributors may be used to endorse or promote products
 derived from this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY DAMIAN STEWART ''AS IS'' AND ANY
 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL DAMIAN STEWART BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "ofxOscMessage.h"
#include "ofParameter.h"
#include "ofThreadChannel.h"

#include "OscTypes.h"
#include "OscPacketListener.h"
#include "UdpSocket.h"

/// \class ofxOscReceiver
/// \brief OSC message receiver which listens on a network port
class ofxOscReceiver : public osc::OscPacketListener {
public:

	ofxOscReceiver();
	ofxOscReceiver(const ofxOscReceiver & mom);
	ofxOscReceiver& operator=(const ofxOscReceiver & mom);
	/// for operator= and copy constructor
	ofxOscReceiver& copy(const ofxOscReceiver& other);

	/// set up the receiver with the port to listen for messages on
	/// \return true is listening was started
	bool setup(int listen_port);
	
	/// stop listening and clear the receiver 
	void clear();

	/// /return true if there are any messages waiting for collection
	bool hasWaitingMessages();

	/// take the next message on the queue of received messages, copy its
	/// details into message, and remove it from the queue
	/// \return false if there are no more messages to be got, otherwise return true
	bool getNextMessage(ofxOscMessage& msg);
	OF_DEPRECATED_MSG("Pass a reference instead of a pointer", bool getNextMessage(ofxOscMessage* msg));
	
	/// try to get waiting message an ofParameter
	/// \return true if message was handled by the given parameter
	bool getParameter(ofAbstractParameter & parameter);

	/// \return current port or 0 if setup was not called
	int getPort();
	
	/// disables port reuse reuse which allows the same port to be used by several sockets
	void disableReuse();

	/// enable broadcast capabilities
	/// usually no need to call this, enabled by default
	void enableReuse();

protected:

	/// process an incoming osc message and add it to the queue
	virtual void ProcessMessage(const osc::ReceivedMessage &m, const osc::IpEndpointName& remoteEndpoint);

private:

	/// socket to listen on, unique for each port
	/// shared between objects is allowReuse is true
	std::unique_ptr<osc::UdpListeningReceiveSocket, std::function<void(osc::UdpListeningReceiveSocket*)>> listen_socket;

	std::thread listen_thread; //< listener thread
	ofThreadChannel<ofxOscMessage> messagesChannel; //< message passing thread channel

	bool allowReuse; //< all the port to be reused by several sockets?
	int listen_port; //< port to listen on
};
