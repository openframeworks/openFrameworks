#pragma once

class ofxTCPSettings {
public:

	std::string address;
	int port;
	bool blocking = false;

	std::string messageDelimiter = "[/TCP]";

};
