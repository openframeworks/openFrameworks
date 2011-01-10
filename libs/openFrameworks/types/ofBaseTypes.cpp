/*
 *  ofBaseTypes.cpp
 *  openFrameworksLib
 *
 *  Created by zachary lieberman on 1/9/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofBaseTypes.h"
#include "ofUtils.h"





//---------------------------------------------------------------------------
ofBaseVideoGrabber::ofBaseVideoGrabber(){
	
}

//---------------------------------------------------------------------------
ofBaseVideoGrabber::~ofBaseVideoGrabber(){
	
}

//---------------------------------------------------------------------------
void ofBaseVideoGrabber::setVerbose(bool bTalkToMe){
	ofLog(OF_LOG_WARNING, "ofBaseVideoGrabber::setVerbose not implemented");
}

//---------------------------------------------------------------------------
void ofBaseVideoGrabber::setDeviceID(int _deviceID){
	ofLog(OF_LOG_WARNING, "ofBaseVideoGrabber::setDeviceID not implemented");
}

//---------------------------------------------------------------------------
void ofBaseVideoGrabber::setDesiredFrameRate(int framerate){
	ofLog(OF_LOG_WARNING, "ofBaseVideoGrabber::setDesiredFrameRate not implemented");
}

//---------------------------------------------------------------------------
void ofBaseVideoGrabber::videoSettings(){
	ofLog(OF_LOG_WARNING, "ofBaseVideoGrabber::videoSettings not implemented");
}

