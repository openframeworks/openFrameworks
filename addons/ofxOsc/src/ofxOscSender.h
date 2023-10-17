// copyright (c) openFrameworks team 2010-2023
// copyright (c) Damian Stewart 2007-2009
#pragma once

#include "OscOutboundPacketStream.h"
#include "OscTypes.h"
#include "UdpSocket.h"

#include "ofParameter.h"
#include "ofxOscBundle.h"

/// \struct ofxOscSenderSettings
/// \brief OSC message sender settings
struct ofxOscSenderSettings {
	std::string host = "localhost"; ///< destination host name/ip
	int port = 0; ///< destination port
	bool broadcast = true; ///< broadcast (aka multicast) ip range support?
	bool silent = false; ///< does not complain if msgs not received
};

/// \class ofxOscSender
/// \brief OSC message sender which sends to a specific host & port
class ofxOscSender {
public:
	ofxOscSender(std::string host = "127.0.0.1", int port = 7970, bool silent = false) {
		setup(host, port, silent);
	}
	~ofxOscSender();
	ofxOscSender(const ofxOscSender & mom);
	ofxOscSender & operator=(const ofxOscSender & mom);
	/// for operator= and copy constructor
	ofxOscSender & copy(const ofxOscSender & other);

	/// set up the sender with the destination host name/ip and port
	/// \return true on success
	bool setup(const std::string & host, int port, bool silent = false);

	/// set up the sender with the given settings
	/// \returns true on success
	bool setup(const ofxOscSenderSettings & settings);

	/// clear the sender, does not clear host or port values
	void clear();

	/// returns true if the sender can send
	bool isReady() const;

	/// send the given message
	/// if wrapInBundle is true (default), message sent in a timetagged bundle
	/// \return true on successfull send
	bool sendMessage(const ofxOscMessage & message, bool wrapInBundle = false);
	bool send(const ofxOscMessage & message, bool wrapInBundle = false) { return sendMessage(message, wrapInBundle); };

	/// send the given bundle
	/// \return true on successfull send
	bool sendBundle(const ofxOscBundle & bundle);
	bool send(const ofxOscBundle & bundle) { return sendBundle(bundle); };

	/// create & send a message with data from an ofParameter
	/// \return true on successfull send
	bool sendParameter(const ofAbstractParameter & parameter);
	bool send(const ofAbstractParameter & parameter) { return sendParameter(parameter); };

	/// \return current host name/ip
	std::string getHost() const;

	/// \return current port
	int getPort() const;

	/// \return the current sender settings
	const ofxOscSenderSettings & getSettings() const;

	/// output stream operator for string conversion and printing
	/// \return host name/ip and port separated by a space
	friend std::ostream & operator<<(std::ostream & os, const ofxOscSender & sender);

private:
	// helper methods for constructing messages
	void appendBundle(const ofxOscBundle & bundle, osc::OutboundPacketStream & p);
	void appendMessage(const ofxOscMessage & message, osc::OutboundPacketStream & p);
	void appendParameter(ofxOscBundle & bundle, const ofAbstractParameter & parameter, const std::string & address);
	void appendParameter(ofxOscMessage & msg, const ofAbstractParameter & parameter, const std::string & address);

	ofxOscSenderSettings settings; ///< current settings
	std::unique_ptr<osc::UdpTransmitSocket> sendSocket; ///< sender socket
};
