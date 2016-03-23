#pragma once

class ofxUDPSettings {
public:

	std::string address;
	unsigned short port;
	bool blocking = false;

	bool reuse = false;

	int sendTimeout = 0;
	int sendBufferSize = 0;
	int receiveTimeout = 0;
	int receiveBufferSize = 0;
	int ttl = 0;

	bool receive = false;
	bool send = false;
	bool broadcast = false;
	bool multicast = false;

};
