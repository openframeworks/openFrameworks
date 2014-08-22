/*
 * ofEmscriptenGrabber.h
 *
 *  Created on: May 12, 2014
 *      Author: arturo
 */
#pragma once

#include "ofBaseTypes.h"
#include "ofTexture.h"
#include "ofPixels.h"

class ofxEmscriptenVideoGrabber: public ofBaseVideoGrabber{
public:
	ofxEmscriptenVideoGrabber();
	~ofxEmscriptenVideoGrabber();

	vector<ofVideoDevice>	listDevices();
	bool	initGrabber(int w, int h);
	void	update();
	bool	isFrameNew();

	unsigned char 	* getPixels();
	ofPixels & 		  getPixelsRef();

	void	close();

	float	getHeight();
	float	getWidth();

	bool setPixelFormat(ofPixelFormat pixelFormat);
	ofPixelFormat getPixelFormat();

	ofTexture * getTexture();

	//should implement!
	void setDeviceID(int _deviceID);
	void setDesiredFrameRate(int framerate);
	void videoSettings();

	void setUsePixels(bool usePixels);

private:
	int id;
	ofTexture texture;
	ofPixels pixels;
	int desiredFramerate;
	bool usePixels;
};
