// copyright (c) openFrameworks team 2010-2017
// copyright (c) damian stewart 2007-2009
#pragma once

#include "OscTypes.h"
#include "OscOutboundPacketStream.h"
#include "UdpSocket.h"

#include "ofxOscBundle.h"
#include "ofParameter.h"
#include "ofParameterGroup.h"

/// \class ofxOscSender
/// \brief OSC message sender which sends to a specific host & port
class ofxOscSender{
public:

	ofxOscSender();
	ofxOscSender(const ofxOscSender & mom);
	ofxOscSender& operator=(const ofxOscSender & mom);
	/// for operator= and copy constructor
	ofxOscSender& copy(const ofxOscSender& other);

	/// set up the sender with the destination host name/ip and port
	void setup(const string &host, int port);
	
	/// clear the sender, does not clear host or port values
	void clear();

	/// send the given message
	/// if wrapInBundle is true (default), message sent in a timetagged bundle
	void sendMessage(const ofxOscMessage& message, bool wrapInBundle=true);
	
	/// send the given bundle
	void sendBundle(const ofxOscBundle& bundle);
	
	/// create & send a message with data from an ofParameter
	void sendParameter(const ofAbstractParameter & parameter);

	/// return current host name/ip or "" if setup was not called
	string getHost();

	/// return current port or 0 if setup was not called
	int getPort();
	
	/// disable broadcast capabilities
	/// usually call this before setup
	void disableBroadcast();

	/// enabled broadcast capabilities
	/// usually no need to call this, enabled by default
	void enableBroadcast();

private:
		
	// helper methods for constructing messages
	void appendBundle(const ofxOscBundle& bundle, osc::OutboundPacketStream& p);
	void appendMessage(const ofxOscMessage& message, osc::OutboundPacketStream& p);
	void appendParameter(ofxOscBundle & bundle, const ofAbstractParameter & parameter, const string &address);
	void appendParameter(ofxOscMessage & msg, const ofAbstractParameter & parameter, const string &address);

	std::unique_ptr<osc::UdpTransmitSocket> sendSocket; //< sender socket
	bool broadcast; //< allow multicast broadcasting ip range?
	string host; //< destination host name/ip
	int port; //< destination port
};
