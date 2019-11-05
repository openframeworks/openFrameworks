#pragma once

class ofxTCPSettings {
public:
	ofxTCPSettings(std::string _address, int _port) {
		address = _address;
		port = _port;
	}
	ofxTCPSettings(int _port) {
		port = _port;
	}

	std::string address;
	int port;
	bool blocking = false;
	bool reuse = false;

	std::string messageDelimiter = "[/TCP]";

};
