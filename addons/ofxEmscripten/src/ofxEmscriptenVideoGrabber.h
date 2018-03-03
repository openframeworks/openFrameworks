/*
 * ofEmscriptenGrabber.h
 *
 *  Created on: May 12, 2014
 *      Author: arturo
 */
#pragma once

#include "ofVideoBaseTypes.h"
#include "ofTexture.h"
#include "ofPixels.h"

class ofxEmscriptenVideoGrabber: public ofBaseVideoGrabber{
public:
	ofxEmscriptenVideoGrabber();
	~ofxEmscriptenVideoGrabber();

	std::vector<ofVideoDevice>	listDevices() const;
	bool	setup(int w, int h);
	bool	isInitialized() const;
	void	update();
	bool	isFrameNew() const;

	ofPixels &  getPixels();
	const ofPixels & getPixels() const;

	void	close();

	float	getHeight() const;
	float	getWidth() const;

	bool setPixelFormat(ofPixelFormat pixelFormat);
	ofPixelFormat getPixelFormat() const;

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
