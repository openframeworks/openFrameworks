/*
 * ofxAndroidVideoGrabber.h
 *
 *  Created on: 17/01/2011
 *      Author: arturo
 */

#pragma once

#include "ofBaseTypes.h"
#include "ofPixels.h"
#include "ofEvents.h"

class ofxAndroidVideoGrabber: public ofBaseVideoGrabber{
public:
	ofxAndroidVideoGrabber();
	~ofxAndroidVideoGrabber();

	//needs implementing
	void	listDevices();
	bool	initGrabber(int w, int h);

	bool	isFrameNew();
	void	update();

	unsigned char 	* getPixels();
	ofPixelsRef		getPixelsRef();

	void	close();

	float	getHeight();
	float	getWidth();

	//should implement!
	void setVerbose(bool bTalkToMe);
	void setDeviceID(int _deviceID);
	void setDesiredFrameRate(int framerate);
	void videoSettings();
	void setPixelFormat(ofPixelFormat pixelFormat);
	ofPixelFormat getPixelFormat();

	// specifics android

	bool setAutoFocus(bool autofocus);

	ofEvent<ofPixels> newFrameE;

	// only to be used internally to resize;
	ofPixelsRef getAuxBuffer();
private:
	int attemptFramerate;
	bool bIsFrameNew;
	bool bGrabberInited;
	ofPixelFormat internalPixelFormat;
	ofPixels pixels;
	ofPixels auxBuffer;
};
