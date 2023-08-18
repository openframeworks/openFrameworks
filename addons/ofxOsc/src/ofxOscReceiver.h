// copyright (c) openFrameworks team 2010-2017
// copyright (c) Damian Stewart 2007-2009
#pragma once

#include "ofxOscMessage.h"
#include "ofParameter.h"
#include "ofThreadChannel.h"

#include "OscTypes.h"
#include "OscPacketListener.h"
#include "UdpSocket.h"

/// \struct ofxOscSenderSettings
/// \brief OSC message sender settings
struct ofxOscReceiverSettings {
	int port = 0;                 ///< port to listen on
    std::string host = "0.0.0.0"; ///< host to listen on
    bool reuse = true;            ///< should the port be reused by other receivers?
	bool start = true;            ///< start listening after setup?
};

/// \class ofxOscReceiver
/// \brief OSC message receiver which listens on a network port
class ofxOscReceiver : public osc::OscPacketListener {
public:

	ofxOscReceiver() {};
	~ofxOscReceiver();
	ofxOscReceiver(const ofxOscReceiver &mom);
	ofxOscReceiver& operator=(const ofxOscReceiver &mom);
	/// for operator= and copy constructor
	ofxOscReceiver& copy(const ofxOscReceiver &other);

    /// set up the receiver to listen for messages through any hosts on the given port
    /// and start listening
    ///
    /// multiple receivers can share the same port if port reuse is
    /// enabled (true by default)
    ///
    /// \return true if listening started
    bool setup(int port) { return setup( "0.0.0.0", port); }
    
    /// set up the receiver to listen for messages on the specific host/ip
    /// and start listening
    ///
    /// multiple receivers can share the same port if port reuse is
    /// enabled (true by default)
    ///
    /// \return true if listening started
    bool setup(std::string host, int port);
    
	/// set up the receiver with the given settings
	///
	/// starts listening if start is true (true by default)
	///
	/// multiple receivers can share the same port if port reuse is
	/// enabled (true by default)
	///
	/// \return true if listening was started or start was not required
	bool setup(const ofxOscReceiverSettings &settings);
	
	/// start listening manually using the current settings
	///
	/// this is not required if you called setup(port)
	/// or setup(settings) with start set to true
	///
	/// \return true if listening started or was already running
	bool start();
	
	/// stop listening, does not clear port value
	void stop();
	
	/// \return true if the receiver is listening
	bool isListening() const;

	/// \return true if there are any messages waiting for collection
	bool hasWaitingMessages() const;

	/// remove a message from the queue and copy it's data into msg
	/// \return false if there are no waiting messages, otherwise return true
	bool getNextMessage(ofxOscMessage& msg);
	OF_DEPRECATED_MSG("Pass a reference instead of a pointer", bool getNextMessage(ofxOscMessage *msg));
	
	/// try to get waiting message an ofParameter
	/// \return true if message was handled by the given parameter
	bool getParameter(ofAbstractParameter &parameter);

	/// \return listening port
	int getPort() const;
	
	/// \return the current receiver settings
	const ofxOscReceiverSettings &getSettings() const;
	
	/// output stream operator for string conversion and printing
	/// \return current port value and "listening" if receiver is listening
	friend std::ostream& operator<<(std::ostream &os, const ofxOscReceiver &receiver);

protected:

	/// process an incoming osc message and add it to the queue
	virtual void ProcessMessage(const osc::ReceivedMessage &m, const osc::IpEndpointName &remoteEndpoint);

private:

	/// socket to listen on, unique for each port
	/// shared between objects if allowReuse is true
	std::unique_ptr<osc::UdpListeningReceiveSocket, std::function<void(osc::UdpListeningReceiveSocket*)>> listenSocket;

	std::thread listenThread; ///< listener thread
	ofThreadChannel<ofxOscMessage> messagesChannel; ///< message passing thread channel

	ofxOscReceiverSettings settings; ///< current settings
};
