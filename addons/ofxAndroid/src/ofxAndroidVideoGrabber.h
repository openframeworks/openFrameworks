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
	vector<ofVideoDevice>	listDevices() const;
	bool	setup(int w, int h);
	bool	isInitialized() const;

	bool	isFrameNew() const;
	void	update();

	ofPixels& getPixels();
	const ofPixels&	getPixels() const;

	void	close();

	float	getHeight() const;
	float	getWidth() const;

	//should implement!
	void setVerbose(bool bTalkToMe);
	void setDeviceID(int _deviceID);
	void setDesiredFrameRate(int framerate);
	void videoSettings();
	bool setPixelFormat(ofPixelFormat pixelFormat);
	ofPixelFormat getPixelFormat() const;

	// specifics android

	bool setAutoFocus(bool autofocus);

	ofTexture *	getTexturePtr();

	ofEvent<ofPixels> newFrameE;

	struct Data;
private:
	bool supportsTextureRendering();

	// only to be used internally to resize;
	ofPixelsRef getAuxBuffer();
	shared_ptr<Data> data;
};
