// copyright (c) openFrameworks team 2012-2017
#pragma once

#include "ofxOscSender.h"
#include "ofxOscReceiver.h"
#include "ofParameter.h"
#include "ofParameterGroup.h"

/// \class ofxOscParamaterSync
/// \brief a high-level sync object for ofParamaters over OSC
class ofxOscParameterSync{
public:

	ofxOscParameterSync();
	~ofxOscParameterSync();

	/// set the parameter group & connection info
	/// the remote and local ports must be different to avoid collisions
	void setup(ofParameterGroup &group, int localPort, const std::string &remoteHost, int remotePort);
	
	/// process any incoming messages
	void update();

private:

	/// parameter change callaback
	void parameterChanged(ofAbstractParameter &parameter);
	
	ofxOscSender sender; ///< sync sender
	ofxOscReceiver receiver; ///< sync receiver
	ofParameterGroup syncGroup; ///< target parameter group
	bool updatingParameter; ///< is a parameter being updated?
};
