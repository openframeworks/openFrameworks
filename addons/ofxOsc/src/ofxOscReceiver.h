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

#include <deque>
#include "ofMain.h"

#ifdef TARGET_WIN32
// threads
#include <windows.h>
#else
// threads
#include <pthread.h>
#endif

// osc
#include "OscTypes.h"
#include "OscPacketListener.h"
#include "UdpSocket.h"

// ofxOsc
#include "ofxOscMessage.h"

class ofxOscReceiver : public osc::OscPacketListener
{
public:
	ofxOscReceiver();
	ofxOscReceiver(const ofxOscReceiver & mom);
	ofxOscReceiver & operator=(const ofxOscReceiver & mom);

	/// listen_port is the port to listen for messages on
	void setup( int listen_port );

	/// returns true if there are any messages waiting for collection
	bool hasWaitingMessages();
	/// take the next message on the queue of received messages, copy its details into message, and
	/// remove it from the queue. return false if there are no more messages to be got, otherwise
	/// return true
	OF_DEPRECATED_MSG("Pass a reference instead of a pointer", bool getNextMessage( ofxOscMessage* msg));
	bool getNextMessage( ofxOscMessage& msg );

	bool getParameter(ofAbstractParameter & parameter);

	/// disables port reuse reuse which allows to use the same port by several sockets
	void disableReuse();

	/// enabled broadcast capabilities (usually no need to call this, enabled by default)
	void enableReuse();

protected:
	/// process an incoming osc message and add it to the queue
	virtual void ProcessMessage( const osc::ReceivedMessage &m, const osc::IpEndpointName& remoteEndpoint );

private:
	void setup(osc::UdpListeningReceiveSocket * socket);
	// shutdown the listener
	void shutdown();

	// start the listening thread
#ifdef TARGET_WIN32
	static DWORD WINAPI startThread( void* ofxOscReceiverInstance );
#else
	static void* startThread( void* ofxOscReceiverInstance );
#endif

	// socket to listen on
	std::unique_ptr<osc::UdpListeningReceiveSocket, std::function<void(osc::UdpListeningReceiveSocket*)>> listen_socket;

	std::thread listen_thread;
	ofThreadChannel<ofxOscMessage> messagesChannel;

	bool allowReuse;
	int listen_port;

};
