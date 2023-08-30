// copyright (c) openFrameworks team 2010-2017
// copyright (c) Damian Stewart 2007-2009
#pragma once

#include "OscTypes.h"
#include "OscOutboundPacketStream.h"
#include "UdpSocket.h"

#include "ofxOscBundle.h"
#include "ofParameter.h"
#include "ofParameterGroup.h"

/// \struct ofxOscSenderSettings
/// \brief OSC message sender settings
struct ofxOscSenderSettings {
	std::string host = "localhost"; ///< destination host name/ip
	int port = 0;                   ///< destination port
	bool broadcast = true;          ///< broadcast (aka multicast) ip range support?
};

/// \class ofxOscSender
/// \brief OSC message sender which sends to a specific host & port
class ofxOscSender{
public:


	ofxOscSender() {}
	~ofxOscSender();
	ofxOscSender(const ofxOscSender &mom);
	ofxOscSender& operator=(const ofxOscSender &mom);
	/// for operator= and copy constructor
	ofxOscSender& copy(const ofxOscSender &other);

	/// set up the sender with the destination host name/ip and port
	/// \return true on success
	bool setup(const std::string &host, int port);

	/// set up the sender with the given settings
	/// \returns true on success
	bool setup(const ofxOscSenderSettings &settings);

	/// clear the sender, does not clear host or port values
	void clear();

	/// send the given message
	/// if wrapInBundle is true (default), message sent in a timetagged bundle
	void sendMessage(const ofxOscMessage &message, bool wrapInBundle=false);

	/// send the given bundle
	void sendBundle(const ofxOscBundle &bundle);

	/// create & send a message with data from an ofParameter
	void sendParameter(const ofAbstractParameter &parameter);

	/// \return current host name/ip
	std::string getHost() const;

	/// \return current port
	int getPort() const;

	/// \return the current sender settings
	const ofxOscSenderSettings &getSettings() const;

	/// output stream operator for string conversion and printing
	/// \return host name/ip and port separated by a space
	friend std::ostream& operator<<(std::ostream &os, const ofxOscSender &sender);

private:

	// helper methods for constructing messages
	void appendBundle(const ofxOscBundle &bundle, osc::OutboundPacketStream &p);
	void appendMessage(const ofxOscMessage &message, osc::OutboundPacketStream &p);
	void appendParameter(ofxOscBundle &bundle, const ofAbstractParameter &parameter, const std::string &address);
	void appendParameter(ofxOscMessage &msg, const ofAbstractParameter &parameter, const std::string &address);

	ofxOscSenderSettings settings; ///< current settings
	std::unique_ptr<osc::UdpTransmitSocket> sendSocket; ///< sender socket
};
