/*
 * ofxOscParameterSync.cpp
 *
 *  Created on: 13/07/2012
 *      Author: arturo
 */

#include "ofxOscParameterSync.h"

ofxOscParameterSync::ofxOscParameterSync() {
	syncGroup = NULL;
	updatingParameter = false;
}

ofxOscParameterSync::~ofxOscParameterSync(){
	if(syncGroup)
		ofRemoveListener(syncGroup->parameterChangedE,this,&ofxOscParameterSync::parameterChanged);
}


void ofxOscParameterSync::setup(ofParameterGroup & group, int localPort, string host, int remotePort){
	syncGroup = &group;
	ofAddListener(group.parameterChangedE,this,&ofxOscParameterSync::parameterChanged);
	sender.setup(host,remotePort);
	receiver.setup(localPort);
}

void ofxOscParameterSync::update(){
	if(receiver.hasWaitingMessages()){
		updatingParameter = true;
		receiver.getParameter(*syncGroup);
		updatingParameter = false;
	}
}

void ofxOscParameterSync::parameterChanged( ofAbstractParameter & parameter ){
	if(updatingParameter) return;
	sender.sendParameter(parameter);
}
