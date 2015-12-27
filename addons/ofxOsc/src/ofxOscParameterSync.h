/*
 * ofxOscParameterSync.h
 *
 *  Created on: 13/07/2012
 *      Author: arturo
 */

#pragma once

#include "ofxOscSender.h"
#include "ofxOscReceiver.h"
#include "ofParameter.h"
#include "ofParameterGroup.h"

class ofxOscParameterSync {
public:
	ofxOscParameterSync();
	~ofxOscParameterSync();

	/// the remote and local ports must be different to avoid collisions
    void setup(ofParameterGroup & group, int localPort, const std::string& remoteHost, int remotePort);
	void update();

private:
	void parameterChanged( ofAbstractParameter & parameter );
	ofxOscSender sender;
	ofxOscReceiver receiver;
	ofParameterGroup syncGroup;
	bool updatingParameter;
};
