#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

struct Tester {
	std::shared_ptr<ofxOscSender> sender_;
	std::shared_ptr<ofxOscReceiver> receiver_;
};

class ofApp : public ofBaseApp{
	
	enum Mode {
		DYNA,
		STOP,
		STABLE
	};
	
	
	Mode mode_ { DYNA };
	long msgs_ { 0 };
	long msgs_out_ { 0 };
	long receivers_ { 0 };
	
	bool dyna_ { true } ;
	const size_t low_port_ { 2000 };
	const size_t high_port_ { 65000 };
	size_t port_ { low_port_ };
	
	std::array<std::shared_ptr<Tester>, 10> testers_;
	
	auto reset_stats() {
		msgs_ = 0;
		msgs_out_ = 0;
	}
	
public:
	void setup() override;
	void update() override;
	void draw() override;
	void keyPressed(int key) override;
	
};
