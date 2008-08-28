/*
 *  ofxOscReceiver.h
 *  openFrameworks OSC addon
 *
 *  damian@frey.co.nz
 *
 */



#ifndef _ofxOscRECEIVER_H
#define _ofxOscRECEIVER_H

#include <deque>
#include <ofMain.h>

#ifdef TARGET_WIN32
// threads
#include <windows.h>
#else
// threads
#include <pthread.h>
#endif

// osc
#include <OscTypes.h>
#include <OscPacketListener.h>
#include <UdpSocket.h>

// ofxOsc
#include "ofxOscMessage.h"

class ofxOscReceiver : public osc::OscPacketListener
{
public:
	ofxOscReceiver();
	~ofxOscReceiver();

	/// listen_port is the port to listen for messages on
	void setup( int listen_port );

	/// returns true if there are any messages waiting for collection
	bool hasWaitingMessages();
	/// take the next message on the queue of received messages, copy its details into message, and
	/// remove it from the queue. return false if there are no more messages to be got, otherwise
	/// return true
	bool getNextMessage( ofxOscMessage* );

protected:
	/// process an incoming osc message and add it to the queue
	virtual void ProcessMessage( const osc::ReceivedMessage &m, const IpEndpointName& remoteEndpoint );

private:

	// start the listening thread
#ifdef TARGET_WIN32
	static DWORD WINAPI startThread( void* listen_socket );
#else
	static void* startThread( void* listen_socket );
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
};

#endif
