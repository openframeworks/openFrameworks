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
#include "ofTypes.h"
#include "ofTexture.h"
#include <jni.h>

class ofxAndroidVideoGrabber: public ofBaseVideoGrabber{
public:
	ofxAndroidVideoGrabber();
	~ofxAndroidVideoGrabber();

	//needs implementing
	vector<ofVideoDevice>	listDevices();
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
	bool setPixelFormat(ofPixelFormat pixelFormat);
	ofPixelFormat getPixelFormat();

	// specifics android

	bool setAutoFocus(bool autofocus);

	ofTexture *	getTexture();
	void loadTexture();
	void reloadTexture();
	void unloadTexture();

	ofEvent<ofPixels> newFrameE;

	// only to be used internally to resize;
	ofPixelsRef getAuxBuffer();

	int attemptFramerate;
private:

	bool supportsTextureRendering();
	bool bIsFrameNew;
	bool bGrabberInited;
	ofPixelFormat internalPixelFormat;
	ofPixels pixels;
	ofPixels auxBuffer;
	ofTexture texture;
	jfloatArray matrixJava;
};
