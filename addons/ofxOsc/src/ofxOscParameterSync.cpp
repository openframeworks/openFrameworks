// copyright (c) openFrameworks team 2012-2023
#include "ofxOscParameterSync.h"

//--------------------------------------------------------------
ofxOscParameterSync::ofxOscParameterSync() {
	updatingParameter = false;
}

//--------------------------------------------------------------
ofxOscParameterSync::~ofxOscParameterSync() {
	ofRemoveListener(syncGroup.parameterChangedE(), this, &ofxOscParameterSync::parameterChanged);
}

//--------------------------------------------------------------
void ofxOscParameterSync::setup(ofParameterGroup & group, int localPort, const std::string & host, int remotePort) {
	syncGroup = group;
	ofAddListener(syncGroup.parameterChangedE(), this, &ofxOscParameterSync::parameterChanged);
	sender.setup(host, remotePort);
	receiver.setup(localPort);
}

//--------------------------------------------------------------
void ofxOscParameterSync::setPassphrase(const std::string & passphrase, int iterations) {
	sender.setPassphrase(passphrase, iterations);
	receiver.setPassphrase(passphrase, iterations);
}

//--------------------------------------------------------------
void ofxOscParameterSync::setRequireEncryption(bool require) {
	receiver.requireEncryption.store(require);
}

//--------------------------------------------------------------
void ofxOscParameterSync::update() {
	if (receiver.hasWaitingMessages()) {
		updatingParameter = true;
		receiver.getParameter(syncGroup);
		updatingParameter = false;
	}
}

//--------------------------------------------------------------
void ofxOscParameterSync::parameterChanged(ofAbstractParameter & parameter) {
	if (updatingParameter) return;
	sender.sendParameter(parameter);
}
