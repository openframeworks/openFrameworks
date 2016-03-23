#pragma once

class ofxTCPSettings {
public:

	std::string address;
	int port;
	bool blocking;

	std::string messageDelimiter = "[/TCP]";

};