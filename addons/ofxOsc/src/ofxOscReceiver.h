// copyright (c) openFrameworks team 2010-2023
// copyright (c) Damian Stewart 2007-2009
#pragma once

#include <atomic>
#include <cstdint>
#include <map>
#include <mutex>
#include <optional>
#include <set>
#include <string>

#include "ofParameter.h"
#include "ofThreadChannel.h"
#include "ofxOscCrypto.h"
#include "ofxOscMessage.h"

#include "OscPacketListener.h"
#include "OscTypes.h"
#include "UdpSocket.h"

/// \struct ofxOscSenderSettings
/// \brief OSC message sender settings
struct ofxOscReceiverSettings {
	int port = 0; ///< port to listen on
	std::string host = "0.0.0.0"; ///< host to listen on
	bool reuse = true; ///< should the port be reused by other receivers?
	bool start = true; ///< start listening after setup?
};

/// \class ofxOscReceiver
/// \brief OSC message receiver which listens on a network port
class ofxOscReceiver : public osc::OscPacketListener {
public:
	ofxOscReceiver() = default;
	
	ofxOscReceiver(int port) {
		setup(port);
	};
	ofxOscReceiver(std::string host, int port) {
		setup(host, port);
	}
	~ofxOscReceiver();
	ofxOscReceiver(const ofxOscReceiver & mom);
	ofxOscReceiver & operator=(const ofxOscReceiver & mom);
	/// for operator= and copy constructor
	ofxOscReceiver & copy(const ofxOscReceiver & other);

	/// set up the receiver to listen for messages through any hosts on the given port
	/// and start listening
	///
	/// multiple receivers can share the same port if port reuse is
	/// enabled (true by default)
	///
	/// \return true if listening started
	bool setup(int port) { return setup("0.0.0.0", port); }

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
	bool setup(const ofxOscReceiverSettings & settings);

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

	/// Seal check for incoming datagrams. The passphrase is stretched with PBKDF2-HMAC-SHA256. The default stretch is 200000. Empty turns it off. The sender needs the same passphrase and the same stretch.
	void setPassphrase(const std::string & passphrase, int iterations = 200000);
	bool isEncrypting() const;
	/// 12 hex characters. Matches a sender that was given the same passphrase and stretch.
	std::string getFingerprint() const;
	/// When set, a datagram that is not sealed with the passphrase is dropped.
	std::atomic<bool> requireEncryption { false };

	/// \return true if there are any messages waiting for collection
	bool hasWaitingMessages() const;

	/// remove a message from the queue and copy it's data into msg
	/// \return false if there are no waiting messages, otherwise return true
	bool getNextMessage(ofxOscMessage & msg);
	[[deprecated("Pass a reference instead of a pointer")]]
	bool getNextMessage(ofxOscMessage * msg);

	/// if there is at tleast 1 message in the queue, pop and swap it into an optional message
	/// \return the std::optional<ofxOscMessage> if there was a message; std::nullopt otherwise
	std::optional<const ofxOscMessage> getMessage();
	ofxOscMessage message_buffer;

	/// try to get waiting message an ofParameter
	/// \return true if message was handled by the given parameter
	bool getParameter(ofAbstractParameter & parameter);

	/// \return listening port
	int getPort() const;

	/// \return the current receiver settings
	const ofxOscReceiverSettings & getSettings() const;

	/// output stream operator for string conversion and printing
	/// \return current port value and "listening" if receiver is listening
	friend std::ostream & operator<<(std::ostream & os, const ofxOscReceiver & receiver);

protected:
	/// process an incoming osc message and add it to the queue
	virtual void ProcessMessage(const osc::ReceivedMessage & m, const osc::IpEndpointName & remoteEndpoint);
	void ProcessPacket(const char * data, int size, const osc::IpEndpointName & remoteEndpoint) override;

private:
	struct ReplayKey {
		std::string host;
		int port = 0;
		std::string salt;
		bool operator<(const ReplayKey & other) const {
			if (host != other.host) {
				return host < other.host;
			}
			if (port != other.port) {
				return port < other.port;
			}
			return salt < other.salt;
		}
	};
	struct ReplaySlot {
		std::uint64_t high = 0;
		std::set<std::uint64_t> seen;
	};

	bool acceptCounter(const std::string & host, int port, const std::string & salt, std::uint64_t counter);
	/// socket to listen on, unique for each port
	/// shared between objects if allowReuse is true
	std::unique_ptr<osc::UdpListeningReceiveSocket, std::function<void(osc::UdpListeningReceiveSocket *)>> listenSocket;

	std::thread listenThread; ///< listener thread
	ofThreadChannel<ofxOscMessage> messagesChannel; ///< message passing thread channel

	ofxOscReceiverSettings settings; ///< current settings
	ofxOsc::crypto::Cipher cipher;
	mutable std::mutex replayMutex;
	std::map<ReplayKey, ReplaySlot> replay;
};
