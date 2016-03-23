#pragma once

/// Socket constants.
#define SOCKET_TIMEOUT			SOCKET_ERROR - 1
#define NO_TIMEOUT				0xFFFF
#define OF_UDP_DEFAULT_TIMEOUT   NO_TIMEOUT

class ofxUDPSettings {
public:

	std::string address;
	unsigned short port;
	bool blocking = false;

	bool reuse = false;

	int sendTimeout = OF_UDP_DEFAULT_TIMEOUT;
	int sendBufferSize = 0;
	int receiveTimeout = OF_UDP_DEFAULT_TIMEOUT;
	int receiveBufferSize = 0;
	int ttl = 1;

	bool receive = false;
	bool send = false;
	bool broadcast = false;
	bool multicast = false;

};
