/*
 * ofOscParameter.h
 *
 *  Created on: 14/02/2012
 *      Author: arturo
 */

#pragma once

#include "ofParameter.h"
#include "ofxOsc.h"
#include "ofThread.h"

template<typename ParameterType>
class ofOscParameter: public ofParameter<ParameterType>{
public:
	ofOscParameter();
	ofOscParameter(ParameterType v);
	ofOscParameter(string name, ParameterType v);

	ParameterType getValue();
	void setValue(ParameterType v);

	static void setup(string remotehost, int remoteport, int localport);

private:
	class OscUpdater: public ofThread{
	public:
		void setup(string remotehost, int remoteport, int localport){
			oscSender.setup(remotehost,remoteport);
			oscReceiver.setup(localport);
			startThread(true,false);
		}

		void threadedFunction(){

		}

	private:
		ofxOscSender oscSender;
		ofxOscReceiver oscReceiver;
		map<string,ofParameter>
	};

	static OscUpdater oscUpdater;
};

