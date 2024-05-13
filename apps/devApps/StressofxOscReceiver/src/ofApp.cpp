#include "ofApp.h"

void ofApp::setup(){
	for (size_t i = 0; i < testers_.size(); i++) {
		testers_[i] = std::make_shared<Tester>();
	}
	ofLogNotice("ready to roll");
	ofSetVerticalSync(false);
}

void ofApp::update(){

	if (dyna_ && ofGetFrameNum()%3==0) {
		for (const auto & tester: testers_) {
			tester->receiver_ = std::make_shared<ofxOscReceiver>(port_);
			if (tester->receiver_->isListening()) {
				tester->sender_ = std::make_shared<ofxOscSender>("127.0.0.1", port_);
				receivers_++;
			}
			if (port_++ > high_port_) port_ = low_port_;
		}
	}

	for (const auto & tester: testers_) {
		for (size_t i=0; i<10; i++) {
			ofxOscMessage m{"/test"};
			m.add(i);
			tester->sender_->sendMessage(m.add(port_).add(i));
			msgs_out_++;
		}
	}

	for (const auto & tester: testers_) {
		while (auto m = tester->receiver_->getMessage()) {
			msgs_++;
		}
	}
}

void ofApp::draw(){
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
	
	ofDrawBitmapString("FPS:   " + ofToString(ofGetFrameRate(), 2), 10, ofGetHeight()-30);
}

void ofApp::keyPressed(int key){
	if (key==' ' ) {
		dyna_ = !dyna_;
		msgs_ = 0;
		msgs_out_ = 0;
	}
}

