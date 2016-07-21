#pragma once

/// Socket constants.
#define NO_TIMEOUT            	0xFFFF
#define OF_UDP_DEFAULT_TIMEOUT	NO_TIMEOUT

class ofxUDPSettings {
public:
	void sendTo(std::string address, unsigned short port) {
		sendAddress = address;
		sendPort = port;
	}
	void receiveOn(std::string address, unsigned short port) {
		bindAddress = address;
		bindPort = port;
	}
	void receiveOn(unsigned short port) {
		bindPort = port;
	}

	std::string sendAddress;
	unsigned short sendPort = 0;
	std::string bindAddress;
	unsigned short bindPort = 0;

	bool blocking = false;
	bool reuse = false;

	int sendTimeout = OF_UDP_DEFAULT_TIMEOUT;
	int sendBufferSize = 0;
	int receiveTimeout = OF_UDP_DEFAULT_TIMEOUT;
	int receiveBufferSize = 0;
	int ttl = 1;

	bool broadcast = false;
	bool multicast = false;

};
