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
	~ofxOscReceiver();

	/// listen_port is the port to listen for messages on
	void setup( int listen_port, bool allowReuse = true );

	/// returns true if there are any messages waiting for collection
	bool hasWaitingMessages();
	/// take the next message on the queue of received messages, copy its details into message, and
	/// remove it from the queue. return false if there are no more messages to be got, otherwise
	/// return true
	bool getNextMessage( ofxOscMessage* );

	bool getParameter(ofAbstractParameter & parameter);

protected:
	/// process an incoming osc message and add it to the queue
	virtual void ProcessMessage( const osc::ReceivedMessage &m, const IpEndpointName& remoteEndpoint );

private:
	// shutdown the listener
	void shutdown();

	// start the listening thread
#ifdef TARGET_WIN32
	static DWORD WINAPI startThread( void* ofxOscReceiverInstance );
#else
	static void* startThread( void* ofxOscReceiverInstance );
#endif
	// queue of osc messages
	std::deque< ofxOscMessage* > messages;

	// socket to listen on
	UdpListeningReceiveSocket* listen_socket;

	// mutex helpers
	void grabMutex();
	void releaseMutex();

#ifdef TARGET_WIN32
	// thread to listen with
	HANDLE thread;
	// mutex for the thread queue
	HANDLE mutex;
#else
	// thread to listen with
	pthread_t thread;
	// mutex for the message queue
	pthread_mutex_t mutex;
#endif
	// ready to be deleted
	bool socketHasShutdown;

};
