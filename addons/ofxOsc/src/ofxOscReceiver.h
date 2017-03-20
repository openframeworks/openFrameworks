// copyright (c) openFrameworks team 2010-2017
// copyright (c) Damian Stewart 2007-2009
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
	~ofxOscReceiver();
	ofxOscReceiver(const ofxOscReceiver &mom);
	ofxOscReceiver& operator=(const ofxOscReceiver &mom);
	/// for operator= and copy constructor
	ofxOscReceiver& copy(const ofxOscReceiver &other);

	/// set up the receiver with the port to listen for messages on
	/// and start listening
	///
	/// multiple receivers can share the same port if port reuse is
	/// enabled (true by default)
	///
	/// \return true if listening started successfully
	bool setup(int port);
	
	/// start listening manually using the current port value
	///
	/// this is not required if you called setup()
	///
	/// \return true if listening started or was already running
	bool start();
	
	/// stop listening, does not clear port value
	void stop();
	
	/// \return true if the receiver is listening
	bool isListening();

	/// \return true if there are any messages waiting for collection
	bool hasWaitingMessages();

	/// take the next message on the queue of received messages, copy its
	/// details into message, and remove it from the queue
	/// \return false if there are no more messages to be got, otherwise return true
	bool getNextMessage(ofxOscMessage& msg);
	OF_DEPRECATED_MSG("Pass a reference instead of a pointer", bool getNextMessage(ofxOscMessage *msg));
	
	/// try to get waiting message an ofParameter
	/// \return true if message was handled by the given parameter
	bool getParameter(ofAbstractParameter &parameter);

	/// set the receiver port manually, does not start listening
	///
	/// restarts receiver if already running
	///
	/// this is not required if you called setup()
	///
	/// multiple receivers can share the same port if port reuse is
	/// enabled (true by default)
	///
	/// \returns true if port set successfully
	bool setPort(int port);

	/// \return listening port or 0 if port has not been set
	int getPort();
	
	/// disables port reuse reuse which allows the same port to be used by several sockets
	void disableReuse();

	/// enable broadcast capabilities
	/// usually no need to call this, enabled by default
	void enableReuse();

protected:

	/// process an incoming osc message and add it to the queue
	virtual void ProcessMessage(const osc::ReceivedMessage &m, const osc::IpEndpointName &remoteEndpoint);

private:

	/// socket to listen on, unique for each port
	/// shared between objects if allowReuse is true
	std::unique_ptr<osc::UdpListeningReceiveSocket, std::function<void(osc::UdpListeningReceiveSocket*)>> listenSocket;

	std::thread listenThread; //< listener thread
	ofThreadChannel<ofxOscMessage> messagesChannel; //< message passing thread channel

	bool allowReuse; //< all the port to be reused by several sockets?
	int port; //< port to listen on
};
