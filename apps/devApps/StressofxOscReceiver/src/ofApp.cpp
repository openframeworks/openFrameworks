#include "ofApp.h"

//#define USE_EXPLICIT_FUNCTION 1

#define INSTANTLY_OUT_OF_SCOPE 1

void ofApp::setup(){
	for (size_t i = 0; i < testers_.size(); i++) {
		testers_[i] = std::make_shared<Tester>();
	}
	ofSetVerticalSync(false);
	
#ifdef INSTANTLY_OUT_OF_SCOPE
	ofLogNotice("will hang if thread problem");
	std::unique_ptr<ofxOscReceiver> r = std::make_unique<ofxOscReceiver>();
	r = std::make_unique<ofxOscReceiver>();
#endif
	ofLogNotice("ready to roll");
	
}

void ofApp::update(){

	if (mode_ == STOP) {
		if (ofGetFrameNum()%6==0) {
			for (const auto & tester: testers_) {
				tester->receiver_->stop();
			}
		} if (ofGetFrameNum()%6==3) {
			for (const auto & tester: testers_) {
				tester->receiver_->start();
			}
		}
	} else if (mode_ == DYNA) {
		if (ofGetFrameNum()%3==0) {
			for (const auto & tester: testers_) {
				tester->receiver_ = std::make_shared<ofxOscReceiver>(port_);
				if (tester->receiver_->isListening()) {
					tester->sender_ = std::make_shared<ofxOscSender>("127.0.0.1", port_);
					receivers_++;
				}
				if (port_++ > high_port_) port_ = low_port_;
			}
		}
	}

	for (const auto & tester: testers_) {
		for (size_t i=0; i<10; i++) {
#if defined(USE_EXPLICIT_FUNCTION)
			ofxOscMessage m;
			m.setAddress("/test");
			m.addIntArg(std::int32_t(i));
			m.addIntArg(port_);
			m.addIntArg(i);
			tester->sender_->sendMessage(m);
#else
			ofxOscMessage m{"/test"};
			m.add(i);
			tester->sender_->sendMessage(m.add(port_).add(i));
#endif
			msgs_out_++;
		}
	}

	for (const auto & tester: testers_) {
		while (auto m = tester->receiver_->getMessage()) {
			msgs_++;
		}
	}
}

void ofApp::draw() {
	for (int i = 0; i < 1000; i++) {
		ofSetColor(ofRandom(0,255), ofRandom(0, 255), ofRandom(0, 255));
		ofDrawRectangle(i *15, 10, 10, 50);
	}
	
	ofSetColor(ofColor::white);
	ofDrawBitmapString("receivers destroyed/created:     " + ofToString(receivers_), 10, 80);
	ofDrawBitmapString("sent:     " + ofToString(msgs_out_), 10, 100);
	ofDrawBitmapString("received: " + ofToString(msgs_), 10, 120);
	auto miss = msgs_out_-msgs_;
	ofDrawBitmapString("= lost:   " + ofToString(miss) + " (" + ofToString((float(miss)/(msgs_out_))*100.0f, 2)+"%)", 10, 140);
	
	ofDrawBitmapString("mode 1: dynamic reallocation every 3 frames (<5% loss)", 10, 200);
	ofDrawBitmapString("mode 2: open/close every 3 frames (normal to have 50% loss)", 10, 220);
	ofDrawBitmapString("mode 3: stable (should have 0% loss)", 10, 240);
	ofDrawBitmapString("current mode: "+ofToString(mode_+1), 10, 260);

	ofDrawBitmapString("FPS (unsync'ed to stress things):   " + ofToString(ofGetFrameRate(), 2), 10, ofGetHeight()-30);
}

void ofApp::keyPressed(int key){
	if (key=='1' ) mode_ = DYNA;
	if (key=='2' ) mode_ = STOP;
	if (key=='3' ) {
		mode_ = STABLE;
		for (const auto & tester: testers_) {
			if (!tester->receiver_->isListening()) {
				tester->receiver_->start();
			}
		}
	}
	reset_stats();
}

