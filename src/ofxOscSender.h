/*
 *  ofxOscSender.h
 *  openFrameworks OSC addon
 *
 *  damian@frey.co.nz
 *
 */

#ifndef _ofxOscSENDER_H
#define _ofxOscSENDER_H

/**

ofxOscSender

an ofxOscSender sends messages to a single host/port

*/

#include "ofxOscMessage.h"

class UdpTransmitSocket;

class ofxOscSender
{
public:
	ofxOscSender();
	~ofxOscSender();

	/// send messages to hostname and port
	void setup( const char* hostname, int port );

	/// send the given message
	void sendMessage( const ofxOscMessage& message );

private:

	UdpTransmitSocket* socket;
};

#endif
